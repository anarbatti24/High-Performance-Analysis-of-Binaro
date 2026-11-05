
# Variables for compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O1 -march=native
TARGET = solverv2

# Default rule (runs when you type 'make')
all: $(TARGET)

# Rule to build the executable
$(TARGET): ../helper.hpp helperv2.hpp helperv2.cpp solverv2.cpp
	$(CXX) $(CXXFLAGS) solverv2.cpp helperv2.cpp -o $(TARGET)

# Clean rule to remove built files
clean:
	rm -f $(TARGET)

.PHONY: all clean
