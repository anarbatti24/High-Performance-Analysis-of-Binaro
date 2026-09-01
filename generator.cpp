#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <string>
#include <fstream>

const int PUZZLE_COUNT = 100'000;
const int SIZE = 10;

using Grid = std::vector<std::vector<int>>;


// ============================================================================
// 1. DEDUCTIVE LOGIC ENGINE
// ============================================================================

bool applyThreeInARowRule(Grid& grid)
{
    bool changed = false;

    auto setCell = [&](int r, int c, int val) {
        if (grid[r][c] == -1) {
            grid[r][c] = val;
            changed = true;
        }
    };

    // Check rows
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {

            // XX.
            if (c + 2 < SIZE &&
                grid[r][c] != -1 &&
                grid[r][c] == grid[r][c + 1] &&
                grid[r][c + 2] == -1)
            {
                setCell(r, c + 2, 1 - grid[r][c]);
            }

            // .XX
            if (c + 2 < SIZE &&
                grid[r][c] == -1 &&
                grid[r][c + 1] != -1 &&
                grid[r][c + 1] == grid[r][c + 2])
            {
                setCell(r, c, 1 - grid[r][c + 1]);
            }

            // X.X
            if (c + 2 < SIZE &&
                grid[r][c] != -1 &&
                grid[r][c] == grid[r][c + 2] &&
                grid[r][c + 1] == -1)
            {
                setCell(r, c + 1, 1 - grid[r][c]);
            }
        }
    }

    // Check columns
    for (int c = 0; c < SIZE; ++c) {
        for (int r = 0; r < SIZE; ++r) {

            // XX.
            if (r + 2 < SIZE &&
                grid[r][c] != -1 &&
                grid[r][c] == grid[r + 1][c] &&
                grid[r + 2][c] == -1)
            {
                setCell(r + 2, c, 1 - grid[r][c]);
            }

            // .XX
            if (r + 2 < SIZE &&
                grid[r][c] == -1 &&
                grid[r + 1][c] != -1 &&
                grid[r + 1][c] == grid[r + 2][c])
            {
                setCell(r, c, 1 - grid[r + 1][c]);
            }

            // X.X
            if (r + 2 < SIZE &&
                grid[r][c] != -1 &&
                grid[r][c] == grid[r + 2][c] &&
                grid[r + 1][c] == -1)
            {
                setCell(r + 1, c, 1 - grid[r][c]);
            }
        }
    }

    return changed;
}


bool applyLineCountRule(Grid& grid)
{
    bool changed = false;

    // Check rows
    for (int r = 0; r < SIZE; ++r) {

        int count0 = 0;
        int count1 = 0;

        for (int c = 0; c < SIZE; ++c) {
            if (grid[r][c] == 0)
                ++count0;

            if (grid[r][c] == 1)
                ++count1;
        }

        // Row has all required 0s
        if (count0 == SIZE / 2) {
            for (int c = 0; c < SIZE; ++c) {
                if (grid[r][c] == -1) {
                    grid[r][c] = 1;
                    changed = true;
                }
            }
        }

        // Row has all required 1s
        if (count1 == SIZE / 2) {
            for (int c = 0; c < SIZE; ++c) {
                if (grid[r][c] == -1) {
                    grid[r][c] = 0;
                    changed = true;
                }
            }
        }
    }

    // Check columns
    for (int c = 0; c < SIZE; ++c) {

        int count0 = 0;
        int count1 = 0;

        for (int r = 0; r < SIZE; ++r) {
            if (grid[r][c] == 0)
                ++count0;

            if (grid[r][c] == 1)
                ++count1;
        }

        // Column has all required 0s
        if (count0 == SIZE / 2) {
            for (int r = 0; r < SIZE; ++r) {
                if (grid[r][c] == -1) {
                    grid[r][c] = 1;
                    changed = true;
                }
            }
        }

        // Column has all required 1s
        if (count1 == SIZE / 2) {
            for (int r = 0; r < SIZE; ++r) {
                if (grid[r][c] == -1) {
                    grid[r][c] = 0;
                    changed = true;
                }
            }
        }
    }

    return changed;
}


bool isDeductivelySolvable(Grid grid)
{
    bool progress = true;

    while (progress) {
        progress = false;

        if (applyThreeInARowRule(grid))
            progress = true;

        if (applyLineCountRule(grid))
            progress = true;
    }

    // If any cells remain unknown, the puzzle wasn't
    // completely solved by deduction.
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (grid[r][c] == -1)
                return false;
        }
    }

    return true;
}


// ============================================================================
// 2. SAFE BOARD GENERATION
// ============================================================================

bool isValidLine(const std::vector<int>& line)
{
    int count0 = 0;
    int count1 = 0;

    for (int cell : line) {
        if (cell == 0)
            ++count0;

        if (cell == 1)
            ++count1;
    }

    if (count0 > SIZE / 2 || count1 > SIZE / 2)
        return false;

    // Check for three consecutive equal values
    for (size_t i = 0; i + 2 < line.size(); ++i) {

        if (line[i] != -1 &&
            line[i] == line[i + 1] &&
            line[i] == line[i + 2])
        {
            return false;
        }
    }

    return true;
}


bool isValidGrid(const Grid& grid)
{
    // Check rows
    for (int r = 0; r < SIZE; ++r) {
        if (!isValidLine(grid[r]))
            return false;
    }

    // Check columns
    for (int c = 0; c < SIZE; ++c) {

        std::vector<int> col(SIZE);

        for (int r = 0; r < SIZE; ++r) {
            col[r] = grid[r][c];
        }

        if (!isValidLine(col))
            return false;
    }

    return true;
}


bool fillBoardRecursive(
    Grid& grid,
    std::mt19937& rng,
    int r,
    int c,
    int& steps)
{
    // Prevent excessive recursion
    if (++steps > 10000)
        return false;

    // Board is completely filled
    if (r == SIZE)
        return true;

    int nextR = (c + 1 == SIZE) ? r + 1 : r;
    int nextC = (c + 1 == SIZE) ? 0 : c + 1;

    std::vector<int> vals = {0, 1};

    std::shuffle(vals.begin(), vals.end(), rng);

    for (int v : vals) {

        grid[r][c] = v;

        if (isValidGrid(grid) &&
            fillBoardRecursive(grid, rng, nextR, nextC, steps))
        {
            return true;
        }

        // Undo choice
        grid[r][c] = -1;
    }

    return false;
}


Grid generateFullBoard(std::mt19937& rng)
{
    while (true) {

        Grid grid(
            SIZE,
            std::vector<int>(SIZE, -1)
        );

        int steps = 0;

        if (fillBoardRecursive(grid, rng, 0, 0, steps))
            return grid;
    }
}


// ============================================================================
// 3. PUZZLE GENERATION
// ============================================================================

Grid generateDeductivePuzzle(
    double targetDifficulty,
    std::mt19937& rng)
{
    // Generate a complete valid solution
    Grid fullBoard = generateFullBoard(rng);

    // Copy it to create the puzzle
    Grid puzzle = fullBoard;

    // Create list of all cells
    std::vector<std::pair<int, int>> cells;

    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            cells.push_back({r, c});
        }
    }

    // Randomize removal order
    std::shuffle(cells.begin(), cells.end(), rng);

    int targetRemoval =
        static_cast<int>(SIZE * SIZE * targetDifficulty);

    int removed = 0;

    for (const auto& cell : cells) {

        if (removed >= targetRemoval)
            break;

        int r = cell.first;
        int c = cell.second;

        int temp = puzzle[r][c];

        // Remove the cell
        puzzle[r][c] = -1;

        // Only keep the removal if the puzzle
        // is still deductively solvable.
        if (!isDeductivelySolvable(puzzle)) {
            puzzle[r][c] = temp;
        }
        else {
            ++removed;
        }
    }

    return puzzle;
}


// ============================================================================
// 4. PRINTING
// ============================================================================

void printGrid(const Grid& grid)
{
    for (int r = 0; r < SIZE; ++r) {

        for (int c = 0; c < SIZE; ++c) {

            if (grid[r][c] == -1)
                std::cout << ". ";
            else
                std::cout << grid[r][c] << " ";
        }

        std::cout << '\n';
    }
}


// ============================================================================
// 5. FILE OUTPUT
// ============================================================================

// Writes a puzzle.
//
// Internally:
//     -1 = unknown
//
// In the file:
//      9 = unknown
//      0 = zero
//      1 = one
//
void writePuzzle(
    std::ofstream& file,
    const Grid& grid)
{
    for (int r = 0; r < SIZE; ++r) {

        for (int c = 0; c < SIZE; ++c) {

            if (grid[r][c] == -1)
                file << 9 << ' ';
            else
                file << grid[r][c] << ' ';
        }

        file << '\n';
    }

    // Blank line between puzzles
    file << '\n';
}


// Writes a completely solved board.
//
// No 9s here — this is our ground truth.
void writeSolution(
    std::ofstream& file,
    const Grid& grid)
{
    for (int r = 0; r < SIZE; ++r) {

        for (int c = 0; c < SIZE; ++c) {
            file << grid[r][c] << ' ';
        }

        file << '\n';
    }

    // Blank line between solutions
    file << '\n';
}


// ============================================================================
// 6. MAIN
// ============================================================================

int main()
{
    std::ofstream puzzleFile("binaro.txt");

    if (!puzzleFile.is_open()) {
        std::cerr << "Could not open binaro.txt\n";
        return -1;
    }

    std::ofstream solutionFile("solutions.txt");

    if (!solutionFile.is_open()) {
        std::cerr << "Could not open solutions.txt\n";
        return -1;
    }


    std::random_device rd;
    std::mt19937 rng(rd());


    std::cout
        << "Generating "
        << PUZZLE_COUNT
        << " puzzles...\n";


    for (int i = 0; i < PUZZLE_COUNT; ++i) {

        // Generate complete board
        Grid fullBoard = generateFullBoard(rng);

        // Make a copy which we'll turn into a puzzle
        Grid puzzle = fullBoard;


        // Create randomized list of cells
        std::vector<std::pair<int, int>> cells;

        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                cells.push_back({r, c});
            }
        }

        std::shuffle(cells.begin(), cells.end(), rng);


        // Remove cells while preserving deductive solvability
        const double targetDifficulty = 0.75;

        int targetRemoval =
            static_cast<int>(SIZE * SIZE * targetDifficulty);

        int removed = 0;


        for (const auto& cell : cells) {

            if (removed >= targetRemoval)
                break;

            int r = cell.first;
            int c = cell.second;

            int temp = puzzle[r][c];

            puzzle[r][c] = -1;

            if (!isDeductivelySolvable(puzzle)) {
                // Removal made the puzzle too difficult
                puzzle[r][c] = temp;
            }
            else {
                ++removed;
            }
        }


        // Print progress
        std::cout
            << "Generated puzzle "
            << (i + 1)
            << " / "
            << PUZZLE_COUNT
            << '\n';


        // Write puzzle and known solution
        writePuzzle(puzzleFile, puzzle);
        writeSolution(solutionFile, fullBoard);
    }


    puzzleFile.close();
    solutionFile.close();


    std::cout
        << "\nSuccessfully generated "
        << PUZZLE_COUNT
        << " puzzles.\n";

    return 0;
}
