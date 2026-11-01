#include "helperv2.hpp"
#include <bitset>
#include <sys/prctl.h>
#include <linux/prctl.h>
#include <fcntl.h>
#include <unistd.h>

long rowEvaluations = 0;
long colEvaluations = 0;

int main() {


	// Try the v2b path first
	std::ifstream file;
	std::ifstream answerKeyFile;
	std::ofstream outputFile;

	fileValidation("binaro.txt", "solutions.txt", "v2", "solutionsv2.txt", file, answerKeyFile, outputFile);

	PuzzleSetV2 puzzles = {};
	puzzles.reserve(100'000);

	readFromFile(file, puzzles);

	int perf_fd = open("/tmp/perf_ctl.fifo", O_WRONLY | O_NONBLOCK);

	// std::cout << "SEcond time" << '\n';

	auto start = std::chrono::steady_clock::now();
	
	if (perf_fd != -1) {
		write(perf_fd, "enable\n", 7);
	}

	//prctl(PR_TASK_PERF_EVENTS_ENABLE);

	// for (long j = 0; j < 10000; j++) {

		for (size_t i = 0; i < puzzles.size(); i++) {
			
			while (!done(puzzles[i])) {
		
				solveRows(puzzles[i]);
				solveCols(puzzles[i]);
			}
		}
	// }

	if (perf_fd != -1) {
	    write(perf_fd, "disable\n", 8);
	    close(perf_fd);
	}

	//prctl(PR_TASK_PERF_EVENTS_DISABLE);

	auto stop = std::chrono::steady_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds> (stop - start);

	//std::cout << "HERE" << '\n';
	writeToFile(outputFile, puzzles);


	outputFile.close();

	std::ifstream testFile("solutionsv2.txt");

	validate(answerKeyFile, testFile);
	std::cout << "TIME TAKEN: " << elapsed << '\n';
	std::cout << "ROW EVALUATIONS: " << rowEvaluations << '\n';
	std::cout << "COL EVALUATIONS: " << colEvaluations << '\n';


	file.close();
	answerKeyFile.close();
}
