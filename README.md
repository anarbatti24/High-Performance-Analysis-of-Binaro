
**Introduction**

Binaro, aka Takuzu, is a puzzle game. You can think about it like Sudoku's (cooler) cousin. You have a 10x10 grid with either 1s, 0s or unknown boxes. The rules are simple, each row/column must have at most 5 ones and 5 zeros and there cannot be 3 consecutive digits, so 111, 000 are not permitted (same for columns).

Over the past few months, Binaro has become my favorite pastime on the train to and from university. I solve each puzzle with the sole goal of doing so as fast as possible. My record thus far is 0:57 seconds. 

During this time, I've also become increasingly interested in High Performance Computing (HPC) and understanding how the software we write interacts with hardware and how we can optimize our code to extract maximum performance from the hardware it runs on.

Merging these two ideas was a very interesting thought to me. Computers do things very fast and so I began thinking about creating a solver with the purpose of getting through 100,000 Binaro puzzles as quickly as possible. And from that, I started my **High Performance Analysis of Binaro**


**Project Information and Direction**

The project itself has been split into 5 versions (planned):

**v1 (complete):**

Version 1 is a naive/brute-force solver, whose main aim is to provide a benchmark for the project. Information regarding the performance of v1 can be found in *Information.txt*, located in the root folder. As a brief summary, however, v1 took around 2.8 seconds to solve 100,000 puzzles, with an average solve time of 28μs/puzzle. It had 201,968,544 branch misses in 3,609134,000 branches and had 14,869,100,397 instructions in 8,405,397,478 cycles, for an instructions per cycle (IpC) ratio of around 1.77.

Additionally, because of how I created my data structures, each puzzle takes up 400 bytes of memory, with the total simulation taking up 40MB 
(400 bytes/puzzle * 100,000 puzzles).

As discussed above, the total memory footprint of the simulation is 40MB. My L3 cache is able to store 24MiB of data, therefore, the bulk of my puzzles will need to live in RAM and will need to get fetched into L1/L2/L3. Using **perf** for profiling, I found that my *llc_miss_rate* (last-level cache miss rate) accounted for 70.5% of total *LLC-Loads*. This means that 70.5% of the time, when my CPU resorted to looking in the L3 cache for data, it couldn't find it and therefore had to make the trip to RAM, a very slow process (comparatively). **v2 addresses this.**

Additionally, v1 solves each puzzle sequentially. It must solve puzzle 1 before going to puzzle 2 and so on. Given that each binaro grid can be solved independently of other grids, it makes sense to parallelize the operation of solving to reduce runtime. **v3 addresses this.**

Lastly, v1 steps through rows/columns sequentially. In other words, I would first need to input data into row 1 and then input data into row 5, even if the same rule could be applied to both. The downside being that each row/column is doing a lot of waiting and we're burning through a lot of clock cycles, ultimately resulting in a higher overall runtime. **v4 addresses this.**


**v2 (in progress):**

Version 2 uses optimized data structures to store data, so instead of having std::array<std::array<int, 10>, 10> holding my grids, I was thinking perhaps I could store each row/column as a single 32 bit integer, uint32_t.

With the optimization explained in the first line of v2, I get the following memory footprint: 

32bits/row * 10row + 32bits/column * 10column = 640 bits/puzzle

640 bits/puzzle * 100,000puzzles = 64Mib = 8MB

As discussed before, my L3 cache is able to store 24MiB = 25MB. Therefore, my entire 100,000 puzzle set can fit within the L3 cache, a few times over actually! The impact being that I wouldn't have to waste time going to RAM to source data, as it will always be within the L3 cache, at worst. Therefore, I would expect to see runtime drop and a lower llc_miss_rate, as I, theoretically, wouldn't need to venture into RAM for any reason.

Another optimization I wanted to implement in v2 is using bitwise operators to make the program branch-less. Doing so would drastically reduce branch misses and CPU stalls, leading to a faster overall runtime.


**v3 (planned):**

Version 3 will use multi-threading to parallelize puzzle completion. My CPU is the Intel i7-13700H, which has 6 performance cores and 8 efficiency cores. The P-cores have hyper-threading so there are 12 P-threads and the E-cores don't have hyper-threading so there are 8 E-threads. Given that spinning up a thread takes a few milliseconds (too slow), I was going to implement a thread pool and have idle threads pull and solve puzzles as required.

Once I get to this point, I'll be very interested to see how the threads (P-thread vs. E-thread) perform and the optimal 'division.' Maybe using just the 12 P-threads is faster than using them with the 8 E-threads...


**v4 (planned):** 

Given the nature of Binaro, different rows can have values inputted independently, meaning that if, in Row 1 and Row 5, I have '00,' I know that I should complete them as Row 1,5 = '1001,' in the same clock cycle. Point being, that I can give a **S**ingle **I**nstruction and alter **M**ultiple **D**ata **(SIMD)**. To implement this, I can use the AVX instruction set via the <immintrin.h> library. The YMM register, which is where AVX instructions are carried out, is able to hold 256 bits of information. Used in tandem with the optimization detailed in v2, I would be able to fit the bulk of the 32bit row/column integers within a single YMM register, meaning that SIMD would seem (on paper) to have a drastic impact on runtime.


**v5 (planned):**

The prior versions explored how each individual optimization benchmarks against v1. Therefore, in v5, I'll integrate all optimizations/techniques together to see the overall impact on factors such as: run-time, cycles, instructions, branch-misses and load-misses.

I expect to see vastly lower (if any) cache miss rates, fewer cycles required to get through the simulation (leading to a higher IpC) and perhaps a lower number of branches and branch misses (if I implement using bitwise operators).


**Other Information:**

1. **perf** was used to profile the different versions.
2. The 100,000 puzzle set was generated once, all versions solved the same set of 100,000 puzzles.
3. A constant CPU clock of 2.1GHz on the P-cores and 1.5 on the E-Cores was set for all runs.
