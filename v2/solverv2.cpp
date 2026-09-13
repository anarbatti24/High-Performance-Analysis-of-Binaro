#include "/home/anarbatti24/Programs/BinaroSolver/v2/helperv2.hpp"
#include <bitset>
#include <sys/prctl.h>
#include <linux/prctl.h>
#include <fcntl.h>
#include <unistd.h>


int main() {

	//CHANGED FILE
	std::ifstream file("/home/anarbatti24/Programs/BinaroSolver/v2/test.txt");
	std::ofstream outputFile("/home/anarbatti24/Programs/BinaroSolver/v2/solutionsv2.txt");
	std::ifstream answerKeyFile("/home/anarbatti24/Programs/BinaroSolver/solutions.txt");

	if (!file.is_open() || !outputFile.is_open() || !answerKeyFile.is_open()) {

		std::cout << "File could not open" << '\n';
		return (-1);
	}


	//std::cout << "HELLO" << '\n';

	PuzzleSetV2 puzzles = {};

	readFromFile(file, puzzles);

	//std::cout << "Size of puzzles: " << puzzles.size() * sizeof(GridV2) << '\n';
	//printPuzzleSetV2(puzzles);

	//puzzles.push_back(grid);

	//	std::cout << "Read in " << puzzles.size() << " puzzles" << '\n';

	//std::cout << puzzles.size() << '\n';

	int perf_fd = open("/tmp/perf_ctl.fifo", O_WRONLY);

	auto start = std::chrono::steady_clock::now();
	
	if (perf_fd != -1) {
		write(perf_fd, "enable\n", 7);
	}

	//prctl(PR_TASK_PERF_EVENTS_ENABLE);

	// for (long j = 0; j < 10000; j++) {

		for (int i = 0; i < puzzles.size(); i++) {
			
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

	std::ifstream testFile("/home/anarbatti24/Programs/BinaroSolver/v2/solutionsv2.txt");

	validate(answerKeyFile, testFile);
	std::cout << "TIME TAKEN: " << elapsed << '\n';

	file.close();
	answerKeyFile.close();
}
