#include "/home/anarbatti24/Programs/BinaroSolver/v2/helperv2.hpp"



/**
 * @brief Function to read from the file
 * 
 * @param file	 The file we're reading from
 * @param puzzle The puzzleSet we're populating
 **/
void readFromFile(std::ifstream& file, PuzzleSetV2& puzzle) {

	int bit;

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

	for (int r = ROWS - 1; r >= 0; r--) {
	
		// Case of doubles e.g. '11x'
		grid.rows[offset - r].zeros |= ((grid.rows[offset - r].ones >> 1 & grid.rows[offset - r].ones >> 2) | (grid.rows[offset - r].ones << 1 & grid.rows[offset - r].ones << 2)) & 0x3FF;	
		grid.rows[offset - r].ones |= ((grid.rows[offset - r].zeros >> 1 & grid.rows[offset - r].zeros >> 2) | (grid.rows[offset - r].zeros << 1 & grid.rows[offset - r].zeros << 2)) & 0x3FF;

//		std::cout << "detecting doubles" << '\n';
		
		// Case of gaps e.g '1x1'
		grid.rows[offset - r].zeros |= ((grid.rows[offset - r].ones << 1) & (~grid.rows[offset - r].ones) & (grid.rows[offset - r].ones >> 1)) & 0x3FF;
		grid.rows[offset - r].ones |= ((grid.rows[offset - r].zeros << 1) & (~grid.rows[offset - r].zeros) & (grid.rows[offset - r].zeros >> 1)) & 0x3FF;
//	
//		std::cout << "detecting gaps" << '\n';

		// Filling in when we have 5 bits in a division
		int onesCount = __builtin_popcount(grid.rows[offset - r].ones & 0x3FF);
		int zerosCount = __builtin_popcount(grid.rows[offset - r].zeros & 0x3FF);

		// Branchless for speed
		uint16_t fillZerosFromOnes = (~grid.rows[offset - r].ones & 0x3FF) * (onesCount == 5);
		uint16_t fillOnesFromZeros = (~grid.rows[offset - r].zeros & 0x3FF) * (zerosCount == 5);

		grid.rows[offset - r].zeros |= fillZerosFromOnes;
		grid.rows[offset - r].ones  |= fillOnesFromZeros;


		uint16_t onesCopy = grid.rows[offset - r].ones & 0x3FF;
		uint16_t zerosCopy = grid.rows[offset - r].zeros & 0x3FF;

		// At this point, I've got a ones-division line and a zeros-division line. Now, I need to isolate each bit in each division and send it to its corresponding column.
		
		// Transmitting the changed row bits to their respective columns for the ones-division
		while (onesCopy != 0) {
		
		
//			std::cout << "sending ones columns" << '\n';
			int bitShift = 31 - __builtin_clz(onesCopy);
			uint16_t isolatedMask = 1 << bitShift;
			
			//std::cout << "Ones rows: " << bitShift << '\n';
			grid.cols[offset - bitShift].ones |= (1 << r);

			onesCopy ^= isolatedMask;

		}


		// Transmitting the changed row bits to their respective columns for the zeros-division
		while (zerosCopy != 0) {

//			std::cout << "sending zeros columns" << '\n';
			int bitShift = 31 - __builtin_clz(zerosCopy);
			uint16_t isolatedMask = 1 << bitShift;



			//std::cout << "zeros rows: " << bitShift << '\n';
			grid.cols[offset - bitShift].zeros |= (1 << r);

			zerosCopy ^= isolatedMask;

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
	
		// Case of doubles e.g. '11x'
		grid.cols[offset - c].zeros |= ((grid.cols[offset - c].ones >> 1 & grid.cols[offset - c].ones >> 2) | (grid.cols[offset - c].ones << 1 & grid.cols[offset - c].ones << 2)) & 0x3FF;	
		grid.cols[offset - c].ones |= ((grid.cols[offset - c].zeros >> 1 & grid.cols[offset - c].zeros >> 2) | (grid.cols[offset - c].zeros << 1 & grid.cols[offset - c].zeros << 2)) & 0x3FF;

		// Case of gaps e.g '1x1'
		grid.cols[offset - c].zeros |= ((grid.cols[offset - c].ones << 1) & (~grid.cols[offset - c].ones) & (grid.cols[offset - c].ones >> 1)) & 0x3FF;
		grid.cols[offset - c].ones |= ((grid.cols[offset - c].zeros << 1) & (~grid.cols[offset - c].zeros) & (grid.cols[offset - c].zeros >> 1)) & 0x3FF;



		// Filling in when we have 5 bits in a division
		int onesCount = __builtin_popcount(grid.cols[offset - c].ones & 0x3FF);
		int zerosCount = __builtin_popcount(grid.cols[offset - c].zeros & 0x3FF);

		// BRanchless for speed
		uint16_t fillZerosFromOnes = (~grid.cols[offset - c].ones & 0x3FF) * (onesCount == 5);
		uint16_t fillOnesFromZeros = (~grid.cols[offset - c].zeros & 0x3FF) * (zerosCount == 5);

		grid.cols[offset - c].zeros |= fillZerosFromOnes;
		grid.cols[offset - c].ones  |= fillOnesFromZeros;



		uint16_t onesCopy = grid.cols[offset - c].ones & 0x3FF;
		uint16_t zerosCopy = grid.cols[offset - c].zeros & 0x3FF;

		// At this point, I've got a ones-division line and a zeros-division line. Now, I need to isolate each bit in each division and send it to its corresponding column.
		
		// Transmitting the changed row bits to their respective columns for the ones-division
		while (onesCopy != 0) {
				
			int bitShift = 31 - __builtin_clz(onesCopy);
			uint16_t isolatedMask = 1 << bitShift;
			
			
			//std::cout << "Ones cols: " << bitShift << '\n';
			grid.rows[offset - bitShift].ones |= (1 << c);

			onesCopy ^= isolatedMask;

		}


		// Transmitting the changed row bits to their respective columns for the zeros-division
		while (zerosCopy != 0) {
		
			int bitShift = 31 - __builtin_clz(zerosCopy);
			uint16_t isolatedMask = 1 << bitShift;

			
			//std::cout << "Zeros cols: " << bitShift << '\n';
			grid.rows[offset - bitShift].zeros |= (1 << c);

			zerosCopy ^= isolatedMask;

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

	//std::cout << "In write file" << '\n';

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
 * 
 * @param puzzleSet The vector of puzzles to print
 **/
void printPuzzleSetV2(const PuzzleSetV2& puzzleSet) {
    std::cout << "=== Printing " << puzzleSet.size() << " Puzzles ===" << '\n';

    for (size_t i = 0; i < puzzleSet.size(); i++) {
        std::cout << "Puzzle #" << i << '\n';
        
        for (int r = ROWS - 1; r >= 0; r--) {
            uint16_t onesRow  = puzzleSet[i].rows[offset - r].ones;
            uint16_t zerosRow = puzzleSet[i].rows[offset - r].zeros;

            for (int c = COLS - 1; c >= 0; c--) {
                int isOne  = (onesRow >> c) & 1;
                int isZero = (zerosRow >> c) & 1;

                if (isOne) {
                    std::cout << "1 ";
                } else if (isZero) {
                    std::cout << "0 ";
                } else {
                    std::cout << ". "; // '.' indicates an unsolved cell
                }
            }
            std::cout << '\n';
        }
        std::cout << "------------------" << '\n';
    }
}
