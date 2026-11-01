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
 * Function to verify that the files used in the project and be opened
 *
 * @param binaro	The string we're passing to hold the binaro file
 * @param solutions	The string we're passing to hold the offical solutions
 * @param filePath	The string we're passing to hold the file path extension (if need be)	
 * @param solutionVX	The string we're passing to hold our solver's solutions
 * @param file		The file that's associated with the binaro file
 * @param answerKeyFile	The file that's associated with the solutions string
 * @param outputFile	The file that's associated with the solutionVX string
 **/
inline void fileValidation(std::string binaro, std::string solutions, std::string filePath, std::string solutionsVX, std::ifstream& file, std::ifstream& answerKeyFile, std::ofstream& outputFile) {

	file.open("../" + binaro);

	// If it fails, try the BinaroSolver path
	if (!file.is_open()) {
		file.clear(); 
		file.open(binaro);
	}

	if (!file.is_open()) {
		std::cout << "Binaro File Couldn't Be Opened" << '\n';

	}

	// Do the exact same thing for the answer key
	answerKeyFile.open("../" + solutions);
	if (!answerKeyFile.is_open()) {
		answerKeyFile.clear();
		answerKeyFile.open(solutions);
	}


	if (!answerKeyFile.is_open()) {
		std::cout << "Answer Key File Couldn't Be Opened" << '\n';

	}

	outputFile.open(solutionsVX);
	if (!outputFile.is_open()) {
		outputFile.clear();
		outputFile.open("/" + filePath + "/" + solutionsVX);
	}

	if (!outputFile.is_open()) {
		std::cout << "Output File Couldn't Be Opened" << '\n';

	}
}


/**
 * @brief Function to compare the output of solutions.txt (the  'offical' solutions) and solutionsvX.txt (The generated solutions by solvervX.cpp)
 *
 * @param answerKeyFile 	"solutions.txt"
 * @param outputFile 		"solutionsvX.txt"
 **/
inline int validate(std::ifstream& answerKeyFile, std::ifstream& outputFile) {

	if (!answerKeyFile.is_open()) {
	
		std::cout << "Could not open answer key file" << '\n';
		return (-1);

	}

	if (!outputFile.is_open()) {
		std::cout << "Could not open test file" << '\n';
		return (-2);
	}

	std::string answerKeyLine;
	std::string testLine;
	
	long incorrect = 0;
	long correct = 0;
	long lineCount = 0;

	while (std::getline(answerKeyFile, answerKeyLine) && std::getline(outputFile, testLine)) {
	
		if (answerKeyLine != testLine) { incorrect++; }
		else { correct++; }
		
		lineCount++;

	}

	std::cout << '\n';
	std::cout << "REPORT" << '\n' << "=======================================" << '\n';
	std::cout << "CORRECT LINES: " << correct << "/" << lineCount << '\n';
	std::cout << "INCORRECT LINES: " << incorrect << "/" <<lineCount << '\n';

	answerKeyFile.close();
	outputFile.close();

	return (0);

}

#endif
