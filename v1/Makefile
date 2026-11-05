# Variables for compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O1 -march=native
TARGET = solverv1

# Default rule (runs when you type 'make')
all: $(TARGET)

# Rule to build the executable
$(TARGET): ../helper.hpp helperv1.hpp helperv1.cpp solverv1.cpp 
	$(CXX) $(CXXFLAGS) solverv1.cpp helperv1.cpp -o $(TARGET)

# Clean rule to remove built files
clean:
	rm -f $(TARGET)

.PHONY: all clean
