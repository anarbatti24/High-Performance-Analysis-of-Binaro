#include "helperv2c.hpp"



/**
 * @brief Function to read from the file
 * 
 * @param file	 The file we're reading from
 * @param puzzle The puzzleSet we're populating
 **/
void readFromFile(std::ifstream& file, PuzzleSetV2c& puzzle) {
	
	puzzle.reserve(100'000);

	std::string line;
	uint128_t currentOnes = 0;
	uint128_t currentZeros = 0;

	int rowCount = 0;

	while (std::getline(file, line)) {

		int colCount = 0;
		for (char c : line) {
			if (c == ' ') { continue; } 

			int bitPosition = (rowCount * 10) + colCount;

			if (c == '0') {
				currentZeros |= ((uint128_t)1 << bitPosition);
				colCount++;
			} 
			else if (c == '1') {
				currentOnes |= ((uint128_t)1 << bitPosition);
				colCount++;
			}
			else if (c == '9') {
				currentZeros |= ((uint128_t)0 << bitPosition);
				currentOnes  |= ((uint128_t)0 << bitPosition);
				colCount++;
			}
		}


		// Only count this as a valid puzzle row if we actually found layout bits
		if (colCount > 0) { rowCount++; }

		if (rowCount == 10) {
		
			puzzle.ones.push_back(currentOnes);
			puzzle.zeros.push_back(currentZeros);

			currentOnes = 0;
			currentZeros = 0;
			rowCount = 0;
		}
	}
}


/**
 * @brief Function to print all puzzles in the PuzzleSet
 * @param puzzleSet The vector of puzzles to print
 **/
void printPuzzleSetV2c(const PuzzleSetV2c& puzzleSet) {

	for (size_t i = 0; i < puzzleSet.size(); i++) {
		
		uint128_t ones = puzzleSet.ones[i];
		uint128_t zeros = puzzleSet.zeros[i];

		for (int bitIndex = 0; bitIndex < SIZE; bitIndex++) {

			// Check if the bit at 'bitIndex' is set in either mask
			bool hasOne  = (ones  >> bitIndex) & 1;
			bool hasZero = (zeros >> bitIndex) & 1;

			if (hasOne) {
				std::cout << "1 ";
			} 
			else if (hasZero) {
				std::cout << "0 ";
			} 
			else {
				std::cout << ". ";
			}

			if ((bitIndex + 1) % 10 == 0) {
				std::cout << '\n';
			}
		}
		std::cout << '\n'; 
	}
}
