#include "/home/anarbatti24/Programs/BinaroSolver/v3/helperv3.hpp"
#include <bitset>
#include <sys/prctl.h>
#include <linux/prctl.h>
#include <fcntl.h>
#include <unistd.h>

uint16_t rowQueue;
uint16_t colQueue;
long rowEvaluations = 0;
long colEvaluations = 0;

int main() {

	//CHANGED FILE
	std::ifstream file("/home/anarbatti24/Programs/BinaroSolver/v3/test.txt");
	std::ofstream outputFile("/home/anarbatti24/Programs/BinaroSolver/v3/solutionsv3.txt");
	std::ifstream answerKeyFile("/home/anarbatti24/Programs/BinaroSolver/solutions.txt");

	if (!file.is_open() || !outputFile.is_open() || !answerKeyFile.is_open()) {

		std::cout << "File could not open" << '\n';
		return (-1);
	}

	PuzzleSetV2 puzzles = {};

	readFromFile(file, puzzles);

	int perf_fd = open("/tmp/perf_ctl.fifo", O_WRONLY);

	auto start = std::chrono::steady_clock::now();

	// Perf control
	if (perf_fd != -1) {
		write(perf_fd, "enable\n", 7);
	}

	// Solving loop
	for (int i = 0; i < puzzles.size(); i++) {

		rowQueue = lineMask;
		colQueue = lineMask;

		while (!done(puzzles[i])) {

			uint16_t eitherIsZero = !(rowQueue | colQueue);
			uint16_t mask = lineMask & (-eitherIsZero);

			rowQueue |= mask;
			colQueue |= mask;

			solveRows(puzzles[i]);
			solveCols(puzzles[i]);
		}
	}

	// Perf control
	if (perf_fd != -1) {
		write(perf_fd, "disable\n", 8);
		close(perf_fd);
	}

	auto stop = std::chrono::steady_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds> (stop - start);

	writeToFile(outputFile, puzzles);

	outputFile.close();

	std::ifstream testFile("/home/anarbatti24/Programs/BinaroSolver/v3/solutionsv3.txt");

	validate(answerKeyFile, testFile);
	std::cout << "TIME TAKEN: " << elapsed << '\n';
	std::cout << "ROW EVALUATIONS: " << rowEvaluations << '\n';
	std::cout << "COL EVALUATIONS: " << colEvaluations << '\n';
	
	file.close();
	answerKeyFile.close();
}
