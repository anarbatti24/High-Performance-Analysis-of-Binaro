#ifndef HELPERV2C_HPP
#define HELPERV2C_HPP

#include <iostream>
#include <array>
#include <fstream>
#include "../helper.hpp"
#include <deque>
#include <algorithm>
#include <iostream>
#include <cstdint>

typedef unsigned __int128 uint128_t;
constexpr int SIZE = 100;



struct PuzzleSetV2c {

	std::vector<uint128_t> ones;
	std::vector<uint128_t> zeros;

	void reserve(size_t n) {
	
		ones.reserve(n);
		zeros.reserve(n);
	}

	size_t size() const { return ones.size(); }
};



/**
 * @brief Function to read from the file
 * 
 * @param file   The file we're reading from
 * @param puzzle The puzzleSet we're populating
 **/
void readFromFile(std::ifstream& file, PuzzleSetV2c& puzzle);



/**
 * @brief Function to print all puzzles in the PuzzleSet to the console
 * 
 * @param puzzleSet The SoA dataset of puzzles to print
 **/
void printPuzzleSetV2c(const PuzzleSetV2c& puzzleSet);



/**
 * Function to write to the file
 *
 * @param file   The file we're writing to
 * @param puzzle The puzzleSet we're writing from
 **/
void writeToFile(std::ofstream& file, const PuzzleSetV2c& puzzle);



/**
 * @brief Function to see whether we're done with a specific grid
 * 
 * @param puzzle The SoA dataset
 * @param index  The index of the puzzle we are checking
 **/
bool done(const PuzzleSetV2c& puzzle, size_t index);



/**
 * Function to solve a specific grid
 *
 * @param puzzle The SoA dataset
 * @param index  The index of the puzzle we are solving
 **/
void solveGrid(PuzzleSetV2c& puzzle, size_t index);

#endif
