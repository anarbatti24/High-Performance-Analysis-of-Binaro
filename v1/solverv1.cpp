#include "helperv1.hpp"
#include <chrono>

//Grid, ROWS, COLS defined in 'helper.hpp' located in root

int main() {

	std::ifstream file("../binaro.txt");
	std::ofstream outputFile("solutionsv1.txt");
	std::ifstream answer_keyFile("../solutions.txt");

	//int solvedPuzzles = 1;

	if (!file.is_open()) {
		std::cout << "Could not open file" << '\n';
		return (1);
	}

	//Grid grid = {};
	PuzzleSet puzzles = {};

	readFromFile(file, puzzles);
	//readFromFile(file, grid);	

	file.close();


	//printGrid(grid);
	//printPuzzleSet(puzzles);

	auto start = std::chrono::steady_clock::now();
		
	for (auto& grid : puzzles) {

		while (!done(grid))  {

			solveConsecutiveCols(grid);
			solveConsecutiveRows(grid);
			solveGapCols(grid);
			solveGapRows(grid);
			iterateCols(grid);
			iterateRows(grid);
		}
		//std::cout << "Puzzles Solved: " << solvedPuzzles++ << "/1000" << '\n';
	}

	auto stop = std::chrono::steady_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds> (stop - start);
	

	//std::cout << '\n';
	//printGrid(grid);
	//printPuzzleSet(puzzles);
	
	writeToFile(outputFile, puzzles);
	outputFile.close();

	std::ifstream testFile("solutionsv1.txt");


	validate(answer_keyFile, testFile);
	std::cout << "TIME TAKEN: " << elapsed << '\n';


	return (0);
}
