#include "/home/anarbatti24/Programs/BinaroSolver/v2/helperv2.hpp"



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
 * @brief Function to solve the rows of a grid
 *
 * @param grid The grid whose rows we are solving
 **/
void solveRows(GridV2& grid) {

	for (int r = ROWS - 1; r >= 0; r--) {

		rowEvaluations++;

		uint16_t oldOnes = grid.rows[r].ones;
		uint16_t oldZeros = grid.rows[r].zeros;

		// Case of doubles e.g. '11x'
		grid.rows[r].zeros |= ((grid.rows[r].ones >> 1 & grid.rows[r].ones >> 2) | (grid.rows[r].ones << 1 & grid.rows[r].ones << 2)) & 0x3FF;  
		grid.rows[r].ones |= ((grid.rows[r].zeros >> 1 & grid.rows[r].zeros >> 2) | (grid.rows[r].zeros << 1 & grid.rows[r].zeros << 2)) & 0x3FF;
		
		// Case of gaps e.g '1x1'
		grid.rows[r].zeros |= ((grid.rows[r].ones << 1) & (~grid.rows[r].ones) & (grid.rows[r].ones >> 1)) & 0x3FF; 
		grid.rows[r].ones |= ((grid.rows[r].zeros << 1) & (~grid.rows[r].zeros) & (grid.rows[r].zeros >> 1)) & 0x3FF;

		// Filling in when we have 5 bits in a division
		int onesCount = __builtin_popcount(grid.rows[r].ones & 0x3FF);
		int zerosCount = __builtin_popcount(grid.rows[r].zeros & 0x3FF);

		// Branchless for speed
		uint16_t fillZerosFromOnes = (~grid.rows[r].ones & 0x3FF) * (onesCount == 5);
		uint16_t fillOnesFromZeros = (~grid.rows[r].zeros & 0x3FF) * (zerosCount == 5);

		grid.rows[r].zeros |= fillZerosFromOnes;
		grid.rows[r].ones  |= fillOnesFromZeros;

		uint16_t newOnes = grid.rows[r].ones & ~oldOnes;
		uint16_t newZeros = grid.rows[r].zeros & ~oldZeros;
		
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
}



/**
 * @brief Function to solve the rows of a grid
 *
 * @param grid The grid whose rows we are solving
 **/
void solveCols(GridV2& grid) {

	for (int c = COLS - 1; c >= 0; c--) {

		colEvaluations++;

		uint16_t oldOnes = grid.cols[c].ones;
		uint16_t oldZeros = grid.cols[c].zeros;

		// Case of doubles e.g. '11x'
		grid.cols[c].zeros |= ((grid.cols[c].ones >> 1 & grid.cols[c].ones >> 2) | (grid.cols[c].ones << 1 & grid.cols[c].ones << 2)) & 0x3FF;	
		grid.cols[c].ones |= ((grid.cols[c].zeros >> 1 & grid.cols[c].zeros >> 2) | (grid.cols[c].zeros << 1 & grid.cols[c].zeros << 2)) & 0x3FF;

		// Case of gaps e.g '1x1'
		grid.cols[c].zeros |= ((grid.cols[c].ones << 1) & (~grid.cols[c].ones) & (grid.cols[c].ones >> 1)) & 0x3FF;
		grid.cols[c].ones |= ((grid.cols[c].zeros << 1) & (~grid.cols[c].zeros) & (grid.cols[c].zeros >> 1)) & 0x3FF;

		// Filling in when we have 5 bits in a division
		int onesCount = __builtin_popcount(grid.cols[c].ones & 0x3FF);
		int zerosCount = __builtin_popcount(grid.cols[c].zeros & 0x3FF);

		// Branchless for speed
		uint16_t fillZerosFromOnes = (~grid.cols[c].ones & 0x3FF) * (onesCount == 5);
		uint16_t fillOnesFromZeros = (~grid.cols[c].zeros & 0x3FF) * (zerosCount == 5);

		grid.cols[c].zeros |= fillZerosFromOnes;
		grid.cols[c].ones  |= fillOnesFromZeros;

		uint16_t newOnes = grid.cols[c].ones & ~oldOnes;
		uint16_t newZeros = grid.cols[c].zeros & ~oldZeros;

		while (newOnes) {
		    unsigned bitShift = std::countr_zero(newOnes);
		    newOnes &= newOnes - 1;

		    grid.rows[bitShift].ones |= (1 << c);
		}

		while (newZeros) {
		    unsigned bitShift = std::countr_zero(newZeros);
		    newZeros &= newZeros - 1;

		    grid.rows[bitShift].zeros |= (1 << c);
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
	
		for (int r =  ROWS - 1; r >= 0; r--) {
		
			uint16_t row = puzzle[i].rows[r].ones;

			for (int c = COLS - 1; c >= 0; c--) {
				
				int bit = (row >> c) & 1;

				file << bit << ' ';

			}
			file << '\n';

		}
		file << '\n';
	}

}

/**
 * @brief Function to print all puzzles in the PuzzleSet to the console
 * 
 * @param puzzleSet The vector of puzzles to print
 **/
void printPuzzleSetV2(const PuzzleSetV2& puzzleSet) {

	for (int i = 0; i < 100'000; i++) {
        
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
