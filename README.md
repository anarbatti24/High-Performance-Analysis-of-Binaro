
**Introduction**
-----------------------------------------------------------

Binaro, aka Takuzu, is a puzzle game. You can think about it like Sudoku's (cooler) cousin. You have a 10x10 grid with either 1s, 0s or unknown boxes. The rules are simple, each row/column must have at most 5 ones and 5 zeros and there cannot be 3 consecutive digits, so '111', '000' are not permitted (same for columns).

Over the past few months, Binaro has become my favorite pastime on the train to and from university. I solve each puzzle with the sole goal of finishing as fast as possible. My record thus far is 57 seconds. 

During this time, I've also become increasingly interested in High Performance Computing (HPC) and understanding how the software that is written interacts with hardware and how code can be optimized to extract maximum performance from the hardware it runs on.

I decided to explore this further by creating a 'solver' with the purpose of getting through 100,000 Binaro puzzles as quickly as possible. 

And from that, I started my **High Performance Analysis of Binaro**


**Results**
-----------------------------------------------------------

Tabular data can be viewed through 'Information.txt'

Graphical Results are below as follows (Updated as new versions are benchmarked):

<p align="center">
  <img src="/images/v1_vs_v2.png" width="900">
</p>


**Project Information and Direction**
-----------------------------------------------------------

The project itself has been split into 5 versions (planned):


**v1 (complete):**

Version 1 is a naive/brute-force solver, whose main aim is to provide a benchmark for the project. Information regarding the performance of v1 can be found in *Information.txt*, located in the root folder. As a brief summary, however, v1 took around 2.8 seconds to solve 100,000 puzzles, with an **average solve time of 28μs/puzzle** (A little faster than my 57 seconds, to say the least). It had 201,968,544 branch misses in 3,609134,000 branches and had 14,869,100,397 instructions in 8,405,397,478 cycles, for an instructions per cycle (IpC) ratio of around 1.77.

Additionally, because of how I created my data structures, each puzzle takes up 400 bytes of memory, with the total simulation taking up 40MB 
(400 bytes/puzzle * 100,000 puzzles).

As discussed above, the total memory footprint of the simulation is 40MB. My L3 cache is able to store 24MiB of data, therefore, the bulk of my puzzles will need to live in RAM and will need to get fetched into L1/L2/L3. Using **perf** for profiling, I found that my *llc_miss_rate* (last-level cache miss rate) accounted for 70.5% of total *LLC-Loads*. This means that 70.5% of the time, when my CPU resorted to looking in the L3 cache for data, it couldn't find it and therefore had to make the trip to RAM, a very slow process (comparatively). **v2 addresses this.**

Additionally, v1 solves each puzzle sequentially. It must solve puzzle 1 before going to puzzle 2 and so on. Given that each binaro grid can be solved independently of other grids, it makes sense to parallelize the operation of solving to reduce total solve time across 100'000 puzzles. **v3 addresses this.**

Lastly, v1 steps through rows/columns sequentially. In other words, I would first need to input data into row 1 and then input data into row 5, even if the same rule could be applied to both. The downside being that each row/column is doing a lot of waiting and we're burning through a lot of clock cycles, ultimately resulting in a higher overall runtime. **v4 addresses this.**


**v2 (complete):**

Version 2 introduced a very important question. How can I shrink the amount of memory needed to represent the puzzles?

Please not the terminology used in this section:

*PuzzleSet* - A data structure that holds all 100'000 puzzles
*Grid*      - A data structure that holds just 1 puzzle
*Line*      - A row or column of a grid
*ones Division* - A data structure that holds just the bits that are set to '1' in a line
*zeros Division* - A data structure that holds just the bits that are set to '0' in a line
*cell* - A singular bit in the Line/Grid

My plan was to use a single uint16_t to represent each Line in a Grid, that way, I could reduce the amount of memory needed and I can utilize bitwise operators to make solving lines faster. Lo-and-behold, this idea didn't work because in a Grid, I need to represent 3 states, minimum. 1s 0s and unknowns. Therefore, I would need at least 2 bits per cell. The problem I found with this was that doing bitwise math would be difficult as now I'd have to find some way to chunk the 2 bits together to treat them as 1...not fun.

Seeing as I needed to represent 3 states, I thought that using a base 3 number would work out perfectly, but I quickly found out that 'bitwise' operators for base 3 numbers include the modulo operator, which is notoriously slow.

The idea of 2 bits per cell still seemed like the best option to me, but it was clear that I couldn't just do it all in one data structure i.e. the uint32_t. Therefore, I figured that splitting up a Line into a ones Division and zeros Divison might satisfy all constraints. For starters, I would be able to shrink memory footprint enormously and use bitwise operators.

Suppose I have the line: 0b10x0110x00

The corresponding ones Division is: 0b1000110000
The corresponding zeros Division is: 0b0101001011

So now, ultimately, I just need to employ bitwise math to calculate the rules for me (consecutive '11' and gaps '1x1') across both divisions. And because of bit-parallelism, any satisfying bits will all be found at the same time (within the same Line).

A problem that I had with v1 was that I was using a nested vector. The inner vector (rows) would have good cache locality as it would be a contiguous chunk in memory, but the different rows (outer vector) would have poor cache locality as its not guaranteed that where they might reside in memory relative to the others.

Therefore, in v2, I decided to make Lines for rows and columns and store those in arrays. Therefore, each row is a contiguous block in memory and so is each column, therefore, I get 20 Lines per puzzle.

If we calculate the total memory footprint now, we get: 32 bits/Line * 20 Line/Grid * 100'000 Grid/PuzzleSet = 8 MB/PuzzleSet (down from 40 MB/Puzzleset in v1)

In binaro, rows can be solved independently of each other and the same can be said for columns. But, rows are needed to solve columns and vice-versa. Therefore, when I solve bits in a row/column, I need to send those bits to their respective columns/rows. 

Let's take rows for example. Say I have identified the following new bits in row 2: 0b10x01x0x01

The formula I concluded was that row bitshift = column and row number = column bitshift.

In other words, column[row bit shift] |= 1 << rowNumber;

Let's solve the example above:

first, I start by isolating the highest bit in the Line, so that I can send it, therefore, I get: 0b10000000

Therefore, I need to send this to column 9 with a bitshift of 2.

A mistake that I made when officially benchmarking was sending the entire line through everytime, NOT just the newly found bits, resulting in redundant operations. When sending just the new bits to ensure no redundant operations, my solve time, unde standard conditions dropped to **4 us/puzzle** (was 8 us/puzzle before).


**v3 (planned):**

Version 3 will use multi-threading to parallelize puzzle completion. My CPU is the Intel i7-13700H, which has 6 performance cores and 8 efficiency cores. The P-cores have hyper-threading so there are 12 P-threads and the E-cores don't have hyper-threading so there are 8 E-threads. Given that spinning up a thread takes a few milliseconds (too slow), I was going to implement a thread pool and have idle threads pull and solve puzzles as required.

Once I get to this point, I'll be very interested to see how the threads (P-thread vs. E-thread) perform and the optimal 'division.' Maybe using just the 12 P-threads is faster than using them with the 8 E-threads...


**v4 (planned):** 

Given the nature of Binaro, different rows can have values inputted independently, meaning that if, in Row 1 and Row 5, I have '00,' I know that I should complete them as Row 1,5 = '1001,' in the same clock cycle. Point being, that I can give a **S**ingle **I**nstruction and alter **M**ultiple **D**ata **(SIMD)**. To implement this, I can use the AVX instruction set via the <immintrin.h> library. The YMM register, which is where AVX instructions are carried out, is able to hold 256 bits of information. Used in tandem with the optimization detailed in v2, I would be able to fit the bulk of the 32bit row/column integers within a single YMM register, meaning that SIMD would seem (on paper) to have a drastic impact on runtime.


**v5 (planned):**

The prior versions explored how each individual optimization benchmarks against v1. Therefore, in v5, I'll integrate all optimizations/techniques together to see the overall impact on factors such as: run-time, cycles, instructions, branch-misses and load-misses.

I expect to see vastly lower (if any) cache miss rates, fewer cycles required to get through the simulation (leading to a higher IpC) and perhaps a lower number of branches and branch misses (if I implement using bitwise operators).


