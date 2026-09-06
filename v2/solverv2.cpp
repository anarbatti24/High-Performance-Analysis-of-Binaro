#include "/home/anarbatti24/Programs/BinaroSolver/v2/helperv2.hpp"
#include <bitset>


int main() {

	std::ifstream file("/home/anarbatti24/Programs/BinaroSolver/binaro.txt");
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


	auto start = std::chrono::steady_clock::now();
	
	for (int i = 0; i < 100000; i++) {
		
		while (!done(puzzles[i])) {
	
			solveRows(puzzles[i]);
			solveCols(puzzles[i]);
		}
	}


	auto stop = std::chrono::steady_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds> (stop - start);

	//std::cout << "HERE" << '\n';
	writeToFile(outputFile, puzzles);


	outputFile.close();

	std::ifstream testFile("/home/anarbatti24/Programs/BinaroSolver/v2/solutionsv2.txt");

	validate(answerKeyFile, testFile);
	std::cout << "TIME TAKEN: " << elapsed << '\n';

	file.close();
	answerKeyFile.close();
}
