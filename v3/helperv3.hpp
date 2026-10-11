#ifndef HELPERV3_HPP
#define HELPERV3_HPP

#include <iostream>
#include <array>
#include <fstream>
#include "/home/anarbatti24/Programs/BinaroSolver/helper.hpp"
#include <deque>
#include <algorithm>

// extern std::deque<int> colQueue;
// extern std::deque<int> rowQueue;
constexpr int offset = 9;
constexpr uint16_t lineMask = 0x3FF;
//extern std::array<bool, 10> inRowQueue;
//extern std::array<bool, 10> inColQueue;
extern uint16_t rowQueue;
extern uint16_t colQueue;
extern long rowEvaluations;
extern long colEvaluations;

/**
 * @brief Function to read from the file
 * 
 * @param file	 The file we're reading from
 * @param puzzle The puzzleSet we're populating
 **/
void readFromFile(std::ifstream& file, PuzzleSetV2& puzzle);


/**
 * @brief Function to solve the rows of a grid
 *
 * @param grid 		The grid whose rows we are solving
 * @param row 		The specific row of the grid we're solving 
 * @param colQueue 	The deque containing the columns that were affected by the row alterations
 **/
void solveRows(GridV2& grid);


/**
 * @brief Function to solve the columns of a grid
 *
 * @param grid 		The grod whose columns we're solving
 * @param col 		The specific column of the grid we're solving
 * @param rowQueue 	The deque containing the rows that were affected by the columns alterations
 **/
void solveCols(GridV2& grid);


/**
 * @brief Function to see whether we're done with the grid
 *
 **/
bool done(GridV2& grid);


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
