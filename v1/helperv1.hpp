#ifndef HELPERV1_HPP
#define HELPERV1_HPP

#include <iostream>
#include <array>
#include <fstream>
#include "../helper.hpp"


using Grid = std::array<std::array<int, COLS>, ROWS>;
using PuzzleSet = std::vector<Grid>;

/**
 * Function to read from file into grid
 *
 * @param file 		The file we're reading from: 'binaro.txt'
 * @param puzzles 	The puzzleSet we're populating
 **/
//void readFromFile(std::ifstream& file, Grid& grid);
void readFromFile(std::ifstream& file, PuzzleSet& puzzles);



/**
 * @brief Function for solving consecutive col numbers i.e. 0 0, 1 1
 *
 * @param grid The grid we're passing to solve
 **/
void solveConsecutiveCols(Grid& grid);



/**
 * @brief Function for solving consecutive row numbers
 * 
 * @param grid The grid we're passing to solve
 **/
void solveConsecutiveRows(Grid& grid);



/**
 * @brief Function for solving gaps in col numbers i.e. 0 # 0, # = 1
 * 
 * @param grid The grid we're passing to solve
**/
void solveGapCols(Grid& grid);



/**
 * @brief Function for solving gaps in row numbers
 * 
 * @param grid the Grid we're passing to solve
 **/
void solveGapRows(Grid& grid);



/**
 * @brief Function to iterate through columns (helper function of solveCol)
 *
 * @param grid
 **/
void iterateCols(Grid& grid);



/**
 * @brief Function to iterate through rows (helper function of solveRow)
 *
 * @param grid
 **/
void iterateRows(Grid& grid);



/**
 * @brief Function to control how long we run for (as long as there are 9s present)
 *
 * @param grid
 **/
bool done(Grid& grid);

/**
 * @brief Function to take the opposite of the current number 0 -> 1, 1 -> 0
 *
 * @param bit  The number we want to take the opposite of
 **/
int opposite(int opposite);

#endif
