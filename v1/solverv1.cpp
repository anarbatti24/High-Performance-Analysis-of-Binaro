#include "helperv1.hpp"
#include <chrono>
#include <fcntl.h>
#include <unistd.h>

//Grid, ROWS, COLS defined in 'helper.hpp' located in root

int main() {


	// Try the v1 path first
	std::ifstream file;

	// Do the exact same thing for the answer key
	std::ifstream answerKeyFile;

	std::ofstream outputFile;

	fileValidation("binaro.txt", "solutions.txt", "v1", "solutionsv1.txt", file, answerKeyFile, outputFile);
	
	PuzzleSet puzzles = {};

	readFromFile(file, puzzles);

	file.close();


	int perf_fd = open("/tmp/perf_ctl.fifo", O_WRONLY | O_NONBLOCK);

	auto start = std::chrono::steady_clock::now();
	
	
	if (perf_fd != -1) {
		write(perf_fd, "enable\n", 7);
	}


	for (auto& grid : puzzles) {

		while (!done(grid))  {

			solveConsecutiveCols(grid);
			solveConsecutiveRows(grid);
			solveGapCols(grid);
			solveGapRows(grid);
			iterateCols(grid);
			iterateRows(grid);
		}
	}



	if (perf_fd != -1) {
	    write(perf_fd, "disable\n", 8);
	    close(perf_fd);
	}

	auto stop = std::chrono::steady_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds> (stop - start);
	

	writeToFile(outputFile, puzzles);
	outputFile.close();

	std::ifstream testFile("solutionsv1.txt");


	validate(answerKeyFile, testFile);
	std::cout << "TIME TAKEN: " << elapsed << '\n';


	return (0);
}
