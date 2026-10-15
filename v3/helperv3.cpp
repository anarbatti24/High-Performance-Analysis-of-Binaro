#include </home/anarbatti24/Programs/BinaroSolver/v3/helperv3.hpp>
#include <concepts>
#include <bit>

/**
 * @brief Function to read from the file
 * 
 * @param file	 The file we're reading from
 * @param puzzle The puzzleSet we're populating
 **/
void readFromFile(std::ifstream& file, PuzzleSetV2& puzzle) {

	int bit;

	puzzle.reserve(100'000);

	while (file >> bit) {
		GridV2 grid = {};

		for (int r = ROWS - 1; r >= 0; r--) {
			for (int c = COLS - 1; c >= 0; c--) {

				if (!(r == ROWS - 1 && c == COLS - 1)) {
					if (!(file >> bit)) {
						break; // File ended unexpectedly mid-puzzle
					}
				}

				uint16_t rowMask = (1u << c);
				uint16_t colMask = (1u << r);

				if (bit == 0) {

					grid.rows[r].zeros |= rowMask;
					grid.cols[c].zeros |= colMask;
					//std::cout << "First one" << '\n';

				}

				else if (bit == 1) {

					grid.rows[r].ones |= rowMask;
					grid.cols[c].ones |= colMask;	
					//std::cout << "Second one" << '\n';
				}
			}
		}
		puzzle.push_back(grid);
	}
}


/**
 * @brief Function to see whether we're done with the grid
 *
 **/
bool done(GridV2& grid) {

	for (int r = 0; r < ROWS; r++) {

		if ((grid.rows[r].ones | grid.rows[r].zeros) != 0b0000001111111111) { return (false); }
	}
	return (true);

}



/**
 * Function to solve grids (combines solving rows and columns into 1 call so we can make the rowQueue and colQueue explicitly local)
 *
 * @param grid	The grid we're solving
 **/
void solveGrid(GridV2& grid) {

	// rowQueue and colQueue are local so we can store them in registers
	uint16_t rowQueue = lineMask;
	uint16_t colQueue = lineMask;
	uint16_t rowsDone = 0;
	uint16_t colsDone = 0;

	// Runs while the grid isn't solved
	while (rowsDone != lineMask) {

		// Re-seeds
		if (!(rowQueue | colQueue)) {
			COUNT(reseedEvaluations);
			rowQueue = ~rowsDone & lineMask;
			colQueue = ~colsDone & lineMask;
		}

		while (rowQueue) {

			COUNT(rowEvaluations);
			unsigned r = std::countr_zero(rowQueue);
			rowQueue &= rowQueue - 1;
			Line& currentRow = grid.rows[r];

			uint16_t ones  = currentRow.ones;
			uint16_t zeros = currentRow.zeros;

			uint16_t oldOnes  = ones;
			uint16_t oldZeros = zeros;

			// Case of doubles e.g. '11x'
			zeros |= ((ones  >> 1 & ones  >> 2) | (ones  << 1 & ones  << 2)) & lineMask;  
			ones  |= ((zeros >> 1 & zeros >> 2) | (zeros << 1 & zeros << 2)) & lineMask;

			// Case of gaps e.g '1x1'
			zeros |= ((ones  << 1) & (~ones)  & (ones  >> 1)); 
			ones  |= ((zeros << 1) & (~zeros) & (zeros >> 1));

			// detecting when we have 5 of a bit in a division
			int onesCount  = __builtin_popcount(ones   & lineMask);
			int zerosCount = __builtin_popcount(zeros  & lineMask);

			// Fills in the other bits when we have 5 of a bit
			zeros |= (~ones  & lineMask)  * (onesCount == 5);
			ones  |= (~zeros & lineMask)  * (zerosCount == 5);

			currentRow.ones = ones;
			currentRow.zeros = zeros;

			uint16_t newOnes  = ones   ^ oldOnes;
			uint16_t newZeros = zeros  ^ oldZeros;

			uint16_t changed = newOnes | newZeros;

			rowsDone |= (uint16_t)((ones | zeros) == lineMask) << r;
			colQueue |= changed & ~colsDone;

			// Transmitting altered rowBits to their respective columns as well		
			while (newOnes) {

				unsigned bitShift = std::countr_zero(newOnes);
				newOnes &= newOnes - 1;
				grid.cols[bitShift].ones |= 1 << r;
			}

			while (newZeros) {

				unsigned bitShift = std::countr_zero(newZeros);
				newZeros &= newZeros - 1;
				grid.cols[bitShift].zeros |= (1 << r);
			}

		}

		while (colQueue) {

			COUNT(colEvaluations);
			unsigned c = std::countr_zero(colQueue);
			colQueue &= colQueue - 1;
			Line& currentCol = grid.cols[c];

			uint16_t ones  = currentCol.ones;
			uint16_t zeros = currentCol.zeros;

			uint16_t oldOnes  = ones;
			uint16_t oldZeros = zeros;

			// Case of doubles e.g. '11x'
			zeros |= ((ones  >> 1 & ones  >> 2) | (ones  << 1 & ones  << 2)) & lineMask;  
			ones  |= ((zeros >> 1 & zeros >> 2) | (zeros << 1 & zeros << 2)) & lineMask;

			// Case of gaps e.g '1x1'
			zeros |= ((ones  << 1) & (~ones)  & (ones  >> 1));
			ones  |= ((zeros << 1) & (~zeros) & (zeros >> 1));

			// detecting when we have 5 of a bit in a division
			int onesCount  = __builtin_popcount(ones   & lineMask);
			int zerosCount = __builtin_popcount(zeros  & lineMask);

			// Fills in the other bits when we have 5 of a bit
			zeros |= (~ones  & lineMask)  * (onesCount == 5);
			ones  |= (~zeros & lineMask)  * (zerosCount == 5);

			currentCol.ones = ones;
			currentCol.zeros = zeros;

			uint16_t newOnes  = ones   ^ oldOnes;
			uint16_t newZeros = zeros  ^ oldZeros;

			uint16_t changed = newOnes | newZeros;

			colsDone |= (uint16_t)((ones | zeros) == lineMask) << c;
			rowQueue |= changed & ~rowsDone;

			while (newOnes) {

				unsigned bitShift = std::countr_zero(newOnes);
				newOnes &= newOnes - 1;
				grid.rows[bitShift].ones |= 1 << c;
			}

			while (newZeros) {

				unsigned bitShift = std::countr_zero(newZeros);
				newZeros &= newZeros - 1;
				grid.rows[bitShift].zeros |= (1 << c);
			}
		}
	}
}



/**
 * Function to write to the file
 *
 * @param file		The file we're writing to
 * @param puzzle 	The puzzleSet we're writing from
 **/
void writeToFile(std::ofstream& file, PuzzleSetV2& puzzle) {

	for (size_t i = 0; i < puzzle.size(); i++) {

		//std::cout << "In first loop" << '\n';
		for (int r =  ROWS - 1; r >= 0; r--) {

			//std::cout << "In second loop" << '\n';
			uint16_t row = puzzle[i].rows[r].ones;

			for (int c = COLS - 1; c >= 0; c--) {


				//std::cout << "In third loop" << '\n';
				int bit = (row >> c) & 1;

				file << bit << ' ';
				//std::cout << "This is bit: " << bit << '\n';

			}
			file << '\n';

		}
		file << '\n';
	}

}

/**
 * @brief Function to print all puzzles in the PuzzleSet to the console
 * @param puzzleSet The vector of puzzles to print
 **/
void printPuzzleSetV2(const PuzzleSetV2& puzzleSet) {

	for (size_t i = 0; i < puzzleSet.size(); i++) {

		for (int r = ROWS - 1; r >= 0; r--) {

			uint16_t onesRow  = puzzleSet[i].rows[r].ones;
			uint16_t zerosRow = puzzleSet[i].rows[r].zeros;

			for (int c = COLS - 1; c >= 0; c--) {

				int isOne  = (onesRow >> c) & 1;
				int isZero = (zerosRow >> c) & 1;

				if (isOne) {
					std::cout << "1 ";
				} 

				else if (isZero) {
					std::cout << "0 ";
				} 

				else {
					std::cout << ". "; 
				}
			}
			std::cout << '\n';
		}
		std::cout << '\n';
	}
}
