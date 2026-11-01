#include "helperv2c.hpp"

/**
 * @brief Function to read from the file
 * 
 * @param file	 The file we're reading from
 * @param puzzle The puzzleSet we're populating
 **/
void readFromFile(std::ifstream& file, PuzzleSetV2& puzzle) {

	int bit;
	int index = 0;

	puzzle.reserve(100'000);

	while (file >> bit) {
		GridV2c grid = {};
		int offset = 0;

		for (int r = ROWS - 1; r >= 0; r--) {

			uint16_t rowMask = (1u << (r + offset));

			if (bit == 0) {

				grid.zeros[index] |= rowMask;

			}

			else if (bit == 1) {

				grid.ones[index] |= rowMask;
			}

			offset += 10;

		}
		index++;
		puzzle.push_back(grid);
	}
}



/**
 * @brief Function to print all puzzles in the PuzzleSet to the console
 * @param puzzleSet The vector of puzzles to print
 **/
void printPuzzleSetV2c(const PuzzleSetV2c& puzzleSet) {

	for (size_t i = 0; i < puzzleSet.size(); i++) {

		GridV2c grid = puzzleSet[i];
		uint128_t ones = grid.ones;
		int bitShift = 127;
		int counter = 0;

		while (ones) {
		
			int bit = (1 & (ones << bitShift));
			counter++;

			if (bit == 1) {
				std::cout << "1";
			}
			else if (bit == 0) {
				std::cout << "0";
			}
			else {
				std::cout << ".";
			}

			if (counter == 9) {
				std::cout << '\n';
				counter = 0;
			}

			bitShift--;
		}
		std::cout << '\n';
	}
}
