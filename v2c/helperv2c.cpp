#include "helperv2c.hpp"
#include <immintrin.h>


/**
 * @brief Function to read from the file
 * 
 * @param file	 	The file we're reading from
 * @param puzzleSet 	The set of puzzles we're populating
 **/
void readFromFile(std::ifstream& file, std::vector<Line>& puzzleSet) {

	puzzleSet.reserve(100'000);

	std::string line;
	uint128_t currentOnes = 0;
	uint128_t currentZeros = 0;

	int rowCount = 0;
	int index = 0;

	while (std::getline(file, line)) {

		int colCount = 0;
		for (char c : line) {
			if (c == ' ') { continue; } 

			int bitPosition = (rowCount * 10) + colCount;

			if (c == '0') {
				currentZeros |= ((uint128_t)1 << bitPosition);
				colCount++;
			} 
			else if (c == '1') {
				currentOnes |= ((uint128_t)1 << bitPosition);
				colCount++;
			}
			else if (c == '9') {
				currentZeros |= ((uint128_t)0 << bitPosition);
				currentOnes  |= ((uint128_t)0 << bitPosition);
				colCount++;
			}
		}


		// Only count this as a valid puzzle row if we actually found bits
		if (colCount > 0) { rowCount++; }

		if (rowCount == 10) {
		
			puzzleSet.push_back({currentOnes, currentZeros});
			currentOnes = 0;
			currentZeros = 0;
			rowCount = 0;
		}
	}
}



/**
 * @brief Function to write to a file
 * 
 * @param file 		The file we're writing to
 * @param puzzleSet 	The puzzle set we're writing from
 **/
void writeToFile(std::ofstream& file, const std::vector<Line>& puzzleSet) {

	for (size_t i = 0; i < puzzleSet.size(); i++) {

		uint128_t ones = puzzleSet[i].ones;

		for (int bitIndex = 0; bitIndex < SIZE; bitIndex++) {
		
			int bit = (ones >> bitIndex) & 1;

			file << bit << ' ';

			if ((bitIndex + 1) % 10 == 0) {
				file << '\n';
			}
		}
		file << '\n';
	}
}



/**
 * @brief Function to print all puzzles in the PuzzleSet
 * 
 * @param puzzleSet The puzzles to print
 **/
// void printPuzzleSetV2c(const PuzzleSetV2c& puzzleSet) {
void printPuzzleSetV2c(const std::vector<Line>& puzzleSet) {

	for (size_t i = 0; i < puzzleSet.size(); i++) {

		uint128_t ones = puzzleSet[i].ones;
		uint128_t zeros = puzzleSet[i].zeros;

		for (int bitIndex = 0; bitIndex < SIZE; bitIndex++) {

			// Check if the bit at 'bitIndex' is set in either mask
			bool hasOne  = (ones  >> bitIndex) & 1;
			bool hasZero = (zeros >> bitIndex) & 1;

			if (hasOne) {
				std::cout << "1 ";
			} 
			else if (hasZero) {
				std::cout << "0 ";
			} 
			else {
				std::cout << ". ";
			}

			if ((bitIndex + 1) % 10 == 0) {
				std::cout << '\n';
			}
		}
		std::cout << '\n'; 
	}
}



constexpr uint128_t repeatRows(uint16_t pat) {
	uint128_t m = 0;
	for (int r = 0; r < 10; ++r)
		m |= uint128_t(pat) << (10 * r);
	return m;
}


constexpr uint128_t GRID_MASK      = repeatRows(0x3FF); 		     // all 100 cells
constexpr uint128_t COL_DBL_DOWN   = (uint128_t(1) << 80) - 1;               // rows 0–7
constexpr uint128_t COL_GAP	   = (GRID_MASK << 10) & (GRID_MASK >> 10);  // rows 1–8
constexpr uint128_t COL_DBL_UP     = GRID_MASK ^ ((uint128_t(1) << 20) - 1); // rows 2–9
constexpr uint128_t COL0           = repeatRows(0x1);                        // bits 0,10,...,90
constexpr uint128_t MASK_DBL_RIGHT = repeatRows(0x0FF); 		     // kills row-bleed on >>
constexpr uint128_t MASK_DBL_LEFT  = repeatRows(0x3FC); 		     // kills row-bleed on <<
constexpr uint128_t MASK_GAP       = repeatRows(0x1FE);


inline uint16_t pext128(uint128_t x, uint128_t mask) {
	
	uint64_t lo = _pext_u64(uint64_t(x),       uint64_t(mask));
	uint64_t hi = _pext_u64(uint64_t(x >> 64), uint64_t(mask >> 64));
	
	int nlo = __builtin_popcountll(uint64_t(mask));
	return uint16_t(lo | (hi << nlo));

}
inline uint128_t pdep128(uint16_t src, uint128_t mask) {
	
	int nlo = __builtin_popcountll(uint64_t(mask));
	uint64_t lo = _pdep_u64(src,        uint64_t(mask));
	uint64_t hi = _pdep_u64(src >> nlo, uint64_t(mask >> 64));
	
	return (uint128_t(hi) << 64) | lo;
}



/**
 * Function to solve a specific grid
 *
 * @param puzzleSet	The puzzles we're trying to solve
 * @param index  	The index of the puzzle we are solving
 **/
void solveGrid(std::vector<Line>& puzzleSet) {

	for (size_t index = 0; index < puzzleSet.size(); index++) {

		uint128_t ones = puzzleSet[index].ones;
		uint128_t zeros = puzzleSet[index].zeros;

		auto applyRows = [&]() {

			zeros |= ((ones  >> 1 & ones  >> 2) & MASK_DBL_RIGHT) | ((ones  << 1 & ones  << 2) & MASK_DBL_LEFT);
			ones  |= ((zeros >> 1 & zeros >> 2) & MASK_DBL_RIGHT) | ((zeros << 1 & zeros << 2) & MASK_DBL_LEFT);
			zeros |= (ones  << 1) & ~ones  & (ones  >> 1) & MASK_GAP;
			ones  |= (zeros << 1) & ~zeros & (zeros >> 1) & MASK_GAP;
		
			for (int r = 0; r < 10; ++r) {
		
				int s = 10 * r;
				uint16_t o = (ones  >> s) & 0x3FF;
				uint16_t z = (zeros >> s) & 0x3FF;
				zeros |= uint128_t((~o & 0x3FF) * (__builtin_popcount(o) == 5)) << s;
				ones  |= uint128_t((~z & 0x3FF) * (__builtin_popcount(z) == 5)) << s;
			}
		};

		auto applyCols = [&]() {
			
			zeros |= ((ones  >> 10 & ones  >> 20) & COL_DBL_DOWN) | ((ones  << 10 & ones  << 20) & COL_DBL_UP);
			ones  |= ((zeros >> 10 & zeros >> 20) & COL_DBL_DOWN) | ((zeros << 10 & zeros << 20) & COL_DBL_UP);
			zeros |= (ones  << 10) & ~ones  & (ones  >> 10) & COL_GAP;
			ones  |= (zeros << 10) & ~zeros & (zeros >> 10) & COL_GAP;
		
			for (int c = 0; c < 10; ++c) {
		
				uint128_t mask = COL0 << c;
				uint16_t o = pext128(ones,  mask);
				uint16_t z = pext128(zeros, mask);
				zeros |= pdep128((~o & 0x3FF) * (__builtin_popcount(o) == 5), mask);
				ones  |= pdep128((~z & 0x3FF) * (__builtin_popcount(z) == 5), mask);
			}
		};
		
		while ((ones | zeros) != GRID_MASK) {

			uint128_t oldO = ones, oldZ = zeros;
			applyRows();
			applyCols();
			if (ones == oldO && zeros == oldZ) break;
		}

		puzzleSet[index].ones = ones;
		puzzleSet[index].zeros = zeros;
	}
}
