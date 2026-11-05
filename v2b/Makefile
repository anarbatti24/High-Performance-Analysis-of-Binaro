

# Variables for compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O1 -march=native
TARGET = solverv2b

# Default rule (runs when you type 'make')
all: $(TARGET)

# Rule to build the executable
$(TARGET): ../helper.hpp helperv2b.hpp helperv2b.cpp solverv2b.cpp
	$(CXX) $(CXXFLAGS) solverv2b.cpp helperv2b.cpp -o $(TARGET)

# Clean rule to remove built files
clean:
	rm -f $(TARGET)

.PHONY: all clean
