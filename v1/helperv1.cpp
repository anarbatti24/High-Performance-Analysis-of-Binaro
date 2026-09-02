#include "/home/anarbatti24/Programs/BinaroSolver/v1/helperv1.hpp"



/**
 * @brief Function to read from a file into the PuzzleSet
 *
 * @param file 		The file we're reading from
 * @param puzzles	The PuzzleSet we're reading into
 **/
void readFromFile(std::ifstream& file, PuzzleSet& puzzles) {
    
	Grid grid;

    	while (file) {
        	for (int r = 0; r < ROWS; r++) {
            		for (int c = 0; c < COLS; c++) {
                		if (!(file >> grid[r][c])) {
                    			return;
				}
			}
		}

        puzzles.push_back(grid);
    }
}



/**
 * @brief Function for solving consecutive col numbers i.e. 0 0, 1 1
 **/
void solveConsecutiveCols(Grid& grid) {

	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS - 1; c++) {
		
			if (grid[r][c] != 9) {
				

				if (grid[r][c] == grid[r][c + 1]) {	

					
					if (c > 0 && c + 2 <= COLS - 1) {
				
						grid[r][c - 1] = opposite(grid[r][c]);
						grid[r][c + 2] = opposite(grid[r][c]);

					}
					
					else if (c == 0) { grid[r][c + 2] = opposite(grid[r][c]); }

					else if (c == 8) { grid[r][c - 1] = opposite(grid[r][c]); }
				}
			}
		}
	}
}



/**
 * @brief Function for solving consecutive row numbers
 **/
void solveConsecutiveRows(Grid& grid) {

	for (int r = 0; r < ROWS - 1; r++) {
		for (int c = 0; c < COLS; c++) {
		
			if (grid[r][c] != 9) {
				

				if (grid[r][c] == grid[r + 1][c]) {	

					
					if (r > 0 && r + 2 <= ROWS - 1) {
				
						grid[r - 1][c] = opposite(grid[r][c]);
						grid[r + 2][c] = opposite(grid[r][c]);

					}
					
					else if (r == 0) { grid[r + 2][c] = opposite(grid[r][c]); }

					else if (r == 8) { grid[r - 1][c] = opposite(grid[r][c]); }
				}
			}
		}
	}
}



/**
 * @brief Function for solving gaps in col numbers i.e. 0 # 0, # = 1
 * 
 * @param grid The grid we're passing to solve
 **/
void solveGapCols(Grid& grid) {

	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS - 2; c++) {
		
			if (grid[r][c] != 9) {
			
				if (grid[r][c] == grid[r][c + 2]) { grid[r][c + 1] = opposite(grid[r][c]); }
			}
		}
	}
}



/**
 * @brief Function for solving gaps in row numbers
 * 
 * @param grid the Grid we're passing to solve
 **/
void solveGapRows(Grid& grid) {

	for (int r = 0; r < ROWS - 2; r++) {
		for (int c = 0; c < COLS; c++) {
		
			if (grid[r][c] != 9) {
			
				if (grid[r][c] == grid[r + 2][c]) { grid[r + 1][c] = opposite(grid[r][c]); }
			}
		}
	}
}



/**
 * @brief Function to iterate through columns (helper function of solveCol)
 *
 * @param grid
 **/
void iterateCols(Grid& grid) {

	for (int r = 0; r < ROWS; r++) {
		
		int oneCounter = 0;
		int zeroCounter = 0;

		for (int c = 0; c < COLS; c++) {
		
			if (grid[r][c] == 1) { oneCounter++; }
			else if (grid[r][c] == 0) { zeroCounter++; }

		}

		if (oneCounter == 5) {
		
			for (int c = 0; c < COLS; c++) {
			
				if (grid[r][c] == 9) { grid[r][c] = 0; }

			}

		}
		else if (zeroCounter == 5) {
		
			for (int c = 0; c < COLS; c++) {

				if (grid[r][c] == 9) { grid[r][c] = 1; }

			}	
		}
	}
}



/**
 * @brief Function to iterate through rows (helper function of solveRow)
 *
 * @param grid
 **/
void iterateRows(Grid& grid) {

	for (int c = 0; c < COLS; c++) {
		
		int oneCounter = 0;
		int zeroCounter = 0;

		for (int r = 0; r < ROWS; r++) {
		
			if (grid[r][c] == 1) { oneCounter++; }
			else if (grid[r][c] == 0) { zeroCounter++; }

		}

		if (oneCounter == 5) {
		
			for (int r = 0; r < ROWS; r++) {
			
				if (grid[r][c] == 9) { grid[r][c] = 0; }

			}

		}
		else if (zeroCounter == 5) {
		
			for (int r = 0; r < ROWS; r++) {

				if (grid[r][c] == 9) { grid[r][c] = 1; }

			}	
		}
	}
}



/**
 * @brief Function to control how long we run for (as long as there are 9s present)
 *
 * @param grid
 **/
bool done(Grid& grid) {

	bool result = true;

	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			if (grid[r][c] == 9) { result = false; }
		}
	}

	return (result);
}

