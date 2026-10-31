#ifndef HELPER_HPP
#define HELPER_HPP

#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <bit>
#include <chrono>

constexpr int ROWS = 10;
constexpr int COLS = 10;
constexpr int offset = 9;



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
