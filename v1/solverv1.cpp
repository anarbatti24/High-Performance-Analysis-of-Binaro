#include "/home/anarbatti24/Programs/BinaroSolver/v1/helperv1.hpp"
#include <chrono>
#include <fcntl.h>
#include <unistd.h>

//Grid, ROWS, COLS defined in 'helper.hpp' located in root

int main() {

	std::ifstream file("/home/anarbatti24/Programs/BinaroSolver/binaro.txt");
	std::ofstream outputFile("/home/anarbatti24/Programs/BinaroSolver/v1/solutionsv1.txt");
	std::ifstream answer_keyFile("/home/anarbatti24/Programs/BinaroSolver/solutions.txt");


	if (!file.is_open()) {
		std::cout << "Could not open file" << '\n';
		return (1);
	}

	PuzzleSet puzzles = {};

	readFromFile(file, puzzles);

	file.close();


	int perf_fd = open("/tmp/perf_ctl.fifo", O_WRONLY);

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

	std::ifstream testFile("/home/anarbatti24/Programs/BinaroSolver/v1/solutionsv1.txt");


	validate(answer_keyFile, testFile);
	std::cout << "TIME TAKEN: " << elapsed << '\n';


	return (0);
}
