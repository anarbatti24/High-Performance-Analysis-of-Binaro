

# Variables for compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O1
TARGET = solverv3

# Default rule (runs when you type 'make')
all: $(TARGET)

# Rule to build the executable
$(TARGET): solverv3.cpp helperv3.cpp helperv3.hpp ../helper.hpp
	$(CXX) $(CXXFLAGS) solverv3.cpp helperv3.cpp -o $(TARGET)

# Clean rule to remove built files
clean:
	rm -f $(TARGET)

.PHONY: all clean
