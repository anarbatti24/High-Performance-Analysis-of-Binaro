#include "helperv2c.hpp"
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <linux/prctl.h>

int main() {
	std::ifstream file;
	std::ifstream answerKeyFile;
	std::ofstream outputFile;

	std::vector<Line> PuzzleSetV2c;

	// Read from the files
	fileValidation("binaro.txt", "solutions.txt", "v2c", "solutionsv2c.txt", file, answerKeyFile, outputFile);

	readFromFile(file, PuzzleSetV2c);

	// Initialize perf control 
	int perf_fd = open("/tmp/perf_ctl.fifo", O_WRONLY);

	auto start = std::chrono::steady_clock::now();

	// Signal perf to start collecting hardware counters
	if (perf_fd != -1) {
		if (write(perf_fd, "enable\n", 7) == -1) { }
	}

	// ACTUAL CALL TO THE SOLVER
	solveGrid(PuzzleSetV2c);

	// Signal perf to stop tracking
	if (perf_fd != -1) {
		if (write(perf_fd, "disable\n", 8) == -1) { }
		close(perf_fd);
	}

	auto stop = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

	writeToFile(outputFile, PuzzleSetV2c);
	outputFile.close();

	std::ifstream testFile("solutionsv2c.txt");
	validate(answerKeyFile, testFile);

	std::cout << "TIME TAKEN: " << elapsed.count() << " ns\n";

	file.close();
	answerKeyFile.close();
	
	return (0);
}
