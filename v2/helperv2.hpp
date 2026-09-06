#ifndef HELPERV2_HPP
#define HELPERV2_HPP

#include <iostream>
#include <array>
#include <fstream>
#include "../helper.hpp"

constexpr uint8_t offset = 9;

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
 * @param grid The grid whose rows we are solving
 **/
void solveRows(GridV2& grid);


/**
 * @brief Function to solve the columns of a grid
 *
 * @param grid
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

