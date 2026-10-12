#ifndef HELPERV3_HPP
#define HELPERV3_HPP

#include <iostream>
#include <array>
#include <fstream>
#include "/home/anarbatti24/Programs/BinaroSolver/helper.hpp"
#include <deque>
#include <algorithm>

constexpr int offset = 9;
constexpr uint16_t lineMask = 0x3FF;

extern long rowEvaluations;
extern long colEvaluations;
extern long reseedEvaluations;

#ifdef COUNT_EVALS
	#define COUNT(c) (++(c))
#else
	#define COUNT(c) ((void)0)
#endif


/**
 * @brief Function to read from the file
 * 
 * @param file	 The file we're reading from
 * @param puzzle The puzzleSet we're populating
 **/
void readFromFile(std::ifstream& file, PuzzleSetV2& puzzle);


/**
 * @brief Function to see whether we're done with the grid
 *
 **/
bool done(GridV2& grid);


/**
 * Function to solve grids (combines solving rows and columns into 1 call so we can make the rowQueue and colQueue explicitly local)
 *
 * @param grid	The grid we're solving
 **/
void solveGrid(GridV2& grid);


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


#endif
