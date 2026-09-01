#ifndef HELPER_HPP
#define HELPER_HPP

#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <vector>

constexpr int ROWS = 10;
constexpr int COLS = 10;

using Grid = std::array<std::array<int, COLS>, ROWS>;
using PuzzleSet = std::vector<Grid>;

/**
 * @brief Function to print the grid
 *
 * @param grid The grid we're printing
 **/
inline void printGrid(const Grid& grid) {

	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			
			if (grid[r][c] == 9) { std::cout << '.' << ' '; }

			else { std::cout << grid[r][c] << ' '; }
		}
		std::cout << '\n';
	}


}



/**
 * @brief Function to print the content of the PuzzleSet
 *
 * @param puzzles The puzzleset we're printing
 **/
inline void printPuzzleSet(const PuzzleSet& puzzles) {

	for (long unsigned int i = 0; i < puzzles.size(); i++) {
		for (int r = 0; r < ROWS; r++) {
			for (int c = 0; c < COLS; c++) {
				std::cout << puzzles[i][r][c] << ' ';
			}
		
			
			std::cout << '\n';
		}
				
		std::cout << '\n';
		std::cout << '\n';
	}

}

/**
 * @brief Function to take the opposite of the current number 0 -> 1, 1 -> 0
 *
 * @param bit  The number we want to take the opposite of
 **/
inline int opposite(int opposite) {

	int opp;
	if (opposite == 0) {
		opp = 1;
	}
	else {
		opp = 0;
	}
	return (opp);
}


/**
 * Function to write to a file
 *
 * @param file 		The file we're writing to
 * @param puzzles 	The PuzzleSet we're writing from
 **/
inline void writeToFile(std::ofstream& file, const PuzzleSet& puzzles) {

	for (long unsigned int i = 0; i < puzzles.size(); i++) {
		for (int r = 0; r < ROWS; r++) {
			for (int c = 0; c < COLS; c++) {
				file << puzzles[i][r][c] << ' ';
			}
			file << '\n';
		}
		file << '\n';
	}

}


/**
 * @brief Function to compare the output of solutions.txt (the  'offical' solutions) and solutionsvX.txt (The generated solutions by solvervX.cpp)
 *
 * @param answer_keyFile 	"solutions.txt"
 * @param testFile 		"solutionsvX.txt"
 **/
inline int validate(std::ifstream& answer_keyFile, std::ifstream& testFile) {

	if (!answer_keyFile.is_open()) {
	
		std::cout << "Could not open answer key file" << '\n';
		return (-1);

	}

	if (!testFile.is_open()) {
		std::cout << "Could not open test file" << '\n';
		return (-2);
	}

	std::string answerKeyLine;
	std::string testLine;
	
	long incorrect = 0;
	long correct = 0;
	long lineCount = 0;

	while (std::getline(answer_keyFile, answerKeyLine) && std::getline(testFile, testLine)) {
	
		if (answerKeyLine != testLine) { incorrect++; }
		else { correct++; }
		
		lineCount++;

	}

	std::cout << '\n';
	std::cout << "REPORT" << '\n' << "=======================================" << '\n';
	std::cout << "CORRECT LINES: " << correct << "/" << lineCount << '\n';
	std::cout << "INCORRECT LINES: " << incorrect << "/" <<lineCount << '\n';

	answer_keyFile.close();
	testFile.close();

	return (0);

}

#endif
