
# Variables for compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O1
TARGET = solverv2

# Default rule (runs when you type 'make')
all: $(TARGET)

# Rule to build the executable
$(TARGET): solverv2.cpp helperv2.cpp helperv2.hpp ../helper.hpp
	$(CXX) $(CXXFLAGS) solverv2.cpp helperv2.cpp -o $(TARGET)

# Clean rule to remove built files
clean:
	rm -f $(TARGET)

.PHONY: all clean
