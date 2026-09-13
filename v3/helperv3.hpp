#ifndef HELPERV3_HPP
#define HELPERV3_HPP

#include <iostream>
#include <array>
#include <fstream>
#include <../helper.hpp>


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
 * @param grid 	The grid whose rows we are solving
 * @param row 	The specific row of the grid we're solving 
 **/
void solveRows(GridV2& grid, Line row);


/**
 * @brief Function to solve the columns of a grid
 *
 * @param grid 	The grod whose columns we're solving
 * @param col 	The specific column of the grid we're solving
 **/
void solveCols(GridV2& grid, Line col);


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
