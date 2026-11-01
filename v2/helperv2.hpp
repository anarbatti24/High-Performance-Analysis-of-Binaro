#ifndef HELPERV2_HPP
#define HELPERV2_HPP

#include <iostream>
#include <array>
#include <fstream>
#include "../helper.hpp"

extern long rowEvaluations;
extern long colEvaluations;


struct Line {

	uint16_t ones = 0b0;
	uint16_t zeros = 0b0;
};

struct GridV2 {

	Line rows[10];
	Line cols[10];
};

using PuzzleSetV2 = std::vector<GridV2>;



/**
 * @brief Function to read from the file
 * 
 * @param file	 The file we're reading from
 * @param puzzle The puzzleSet we're populating
 **/
void readFromFile(std::ifstream& file, PuzzleSetV2& puzzle);



/**
 * Function to write to the file
 *
 * @param file		The file we're writing to
 * @param puzzle 	The puzzleSet we're writing from
 **/
void writeToFile(std::ofstream& file, PuzzleSetV2& puzzle);



/**
 * @brief Function to print all puzzles in the PuzzleSet to the console
 * 
 * @param puzzleSet The vector of puzzles to print
 **/
void printPuzzleSetV2(const PuzzleSetV2& puzzleSet);



/**
 * @brief Function to see whether we're done with the grid
 *
 **/
bool done(GridV2& grid);


/**
 * @brief Function to solve the rows of a grid
 *
 * @param grid The grid whose rows we are solving
 **/
void solveRows(GridV2& grid);



/**
 * @brief Function to solve the columns of a grid
 *
 * @param grid
 **/
void solveCols(GridV2& grid);

#endif

