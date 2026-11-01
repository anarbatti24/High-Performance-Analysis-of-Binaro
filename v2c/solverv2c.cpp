#include "helperv2c.hpp"
#include <bitset>
#include <sys/prctl.h>
#include <linux/prctl.h>
#include <fcntl.h>
#include <unistd.h>

int main() {

	std::ifstream file;
	std::ifstream answerKeyFile;
	std::ofstream outputFile;

	fileValidation("binaro.txt", "solutions.txt", "v2c", "solutionsv2c.txt", file, answerKeyFile, outputFile);


	PuzzleSetV2c puzzles = {};

	readFromFile(file, puzzles);

	printPuzzleSetV2c(puzzles);
}
