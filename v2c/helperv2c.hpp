#ifndef HELPERV2C_HPP
#define HELPERV2C_HPP

#include <iostream>
#include <fstream>
#include "../helper.hpp"
#include <cstdint>
#include <bitset>
#include <bit>

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
 * @param file	 	The file we're reading from
 * @param puzzleSet 	The set of puzzles we're populating
 **/
void readFromFile(std::ifstream& file, PuzzleSetV2c& puzzleSet);



/**
 * @brief Function to write to a file
 * 
 * @param file 		The file we're writing to
 * @param puzzleSet 	The puzzle set we're writing from
 **/
void writeToFile(std::ofstream& file, const PuzzleSetV2c& puzzleSet);



/**
 * @brief Function to print all puzzles in the PuzzleSet
 * 
 * @param puzzleSet The puzzles to print
 **/
void printPuzzleSetV2c(const PuzzleSetV2c& puzzleSet);



/**
 * Function to solve a specific grid
 *
 * @param puzzleSet	The puzzles we're trying to solve
 * @param index  	The index of the puzzle we are solving
 **/
void solveGrid(PuzzleSetV2c& puzzleSet, size_t index);



/**
 * @brief Function to print the uint128_t since GCC doesn't natively support it :(
 *
 * @aram data	The uint128_t we're trying to print
 **/
inline void __print(uint128_t data) {

	uint64_t upperBits = static_cast<uint64_t> (data >> 64);
	uint64_t lowerBits = static_cast<uint64_t> (data);

	std::cout << "0b" << std::bitset<64>(upperBits) << std::bitset<64>(lowerBits) << '\n';

}
#endif
