#include </home/anarbatti24/Programs/BinaroSolver/v3/helperv3.hpp>

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

					grid.rows[offset - r].zeros |= rowMask;
					grid.cols[offset - c].zeros |= colMask;
					//std::cout << "First one" << '\n';

				}

				else if (bit == 1) {

					grid.rows[offset - r].ones |= rowMask;
					grid.cols[offset - c].ones |= colMask;	
					//std::cout << "Second one" << '\n';
				}
			}
		}
		puzzle.push_back(grid);
	}
}




/**
 * @brief Function to solve the rows of a grid
 *
 * @param grid The grid whose rows we are solving
 **/
void solveRows(GridV2& grid) {
	
	while (rowQueue > 0) {

		rowEvaluations++;
		auto r = __builtin_ctz(rowQueue);
		rowQueue &= (rowQueue - 1);
		
		auto& currentRow = grid.rows[offset - r];

		uint16_t oldOnes = currentRow.ones;
		uint16_t oldZeros = currentRow.zeros;

		// Case of doubles e.g. '11x'
		currentRow.zeros |= ((currentRow.ones  >> 1 & currentRow.ones  >> 2) | (currentRow.ones  << 1 & currentRow.ones  << 2)) & lineMask;  
		currentRow.ones  |= ((currentRow.zeros >> 1 & currentRow.zeros >> 2) | (currentRow.zeros << 1 & currentRow.zeros << 2)) & lineMask;

		// Case of gaps e.g '1x1'
		currentRow.zeros |= ((currentRow.ones  << 1) & (~currentRow.ones)  & (currentRow.ones  >> 1)) & lineMask; 
		currentRow.ones  |= ((currentRow.zeros << 1) & (~currentRow.zeros) & (currentRow.zeros >> 1)) & lineMask;

		// detecting when we have 5 of a bit in a division
		int onesCount  = __builtin_popcount(currentRow.ones   & lineMask);
		int zerosCount = __builtin_popcount(currentRow.zeros  & lineMask);

		// Fills in the other bits when we have 5 of a bit
		currentRow.zeros |= (~currentRow.ones  & lineMask)  * (onesCount == 5);
		currentRow.ones  |= (~currentRow.zeros & lineMask)  * (zerosCount == 5);

		uint16_t newOnes  = currentRow.ones   ^ oldOnes;
		uint16_t newZeros = currentRow.zeros  ^ oldZeros;

		// Transmitting altered rowBits to their respective columns as well		
		while (newOnes) {

			unsigned bitShift = std::countr_zero(newOnes);
			newOnes &= newOnes - 1;
			grid.cols[offset - bitShift].ones |= 1 << r;
			colQueue |= 1u << (offset - bitShift);
		}

		while (newZeros) {
	
			unsigned bitShift = std::countr_zero(newZeros);
			newZeros &= newZeros - 1;
			grid.cols[offset - bitShift].zeros |= (1 << r);
			colQueue |= 1u << (offset - bitShift);
		}
	}
}


/**
 * @brief Function to solve the columns of a grid
 *
 * @param grid The grid whose columns we are solving
 **/
void solveCols(GridV2& grid) {

	while (colQueue > 0) {

		colEvaluations++;
		auto c = __builtin_ctz(colQueue);
		colQueue &= (colQueue - 1);

		auto& currentCol = grid.cols[offset - c];

		uint16_t oldOnes = currentCol.ones;
		uint16_t oldZeros = currentCol.zeros;

		// Case of doubles e.g. '11x'
		currentCol.zeros |= ((currentCol.ones  >> 1 & currentCol.ones  >> 2) | (currentCol.ones  << 1 & currentCol.ones  << 2)) & lineMask;  
		currentCol.ones  |= ((currentCol.zeros >> 1 & currentCol.zeros >> 2) | (currentCol.zeros << 1 & currentCol.zeros << 2)) & lineMask;

		// Case of gaps e.g '1x1'
		currentCol.zeros |= ((currentCol.ones  << 1) & (~currentCol.ones)  & (currentCol.ones  >> 1)) & lineMask; 
		currentCol.ones  |= ((currentCol.zeros << 1) & (~currentCol.zeros) & (currentCol.zeros >> 1)) & lineMask;

		// detecting when we have 5 of a bit in a division
		int onesCount  = __builtin_popcount(currentCol.ones   & lineMask);
		int zerosCount = __builtin_popcount(currentCol.zeros  & lineMask);

		// Fills in the other bits when we have 5 of a bit
		currentCol.zeros |= (~currentCol.ones  & lineMask)  * (onesCount == 5);
		currentCol.ones  |= (~currentCol.zeros & lineMask)  * (zerosCount == 5);

		uint16_t newOnes  = currentCol.ones   ^ oldOnes;
		uint16_t newZeros = currentCol.zeros  ^ oldZeros;

		// Transmitting altered rowBits to their respective rows as well		
		while (newOnes) {

			unsigned bitShift = std::countr_zero(newOnes);
			newOnes &= newOnes - 1;
			grid.rows[offset - bitShift].ones |= 1 << c;
			rowQueue |= 1u << (offset - bitShift);
		}

		while (newZeros) {
	
			unsigned bitShift = std::countr_zero(newZeros);
			newZeros &= newZeros - 1;
			grid.rows[offset - bitShift].zeros |= (1 << c);
			rowQueue |= 1u << (offset - bitShift);
		}
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
			uint16_t row = puzzle[i].rows[offset - r].ones;

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

	for (int i = 0; i < puzzleSet.size(); i++) {

		for (int r = ROWS - 1; r >= 0; r--) {

			uint16_t onesRow  = puzzleSet[i].rows[offset - r].ones;
			uint16_t zerosRow = puzzleSet[i].rows[offset - r].zeros;

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
