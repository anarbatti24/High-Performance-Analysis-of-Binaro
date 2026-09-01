# Variables for compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O3
TARGET = generator

# Default rule (runs when you type 'make')
all: $(TARGET)

# Rule to build the executable
$(TARGET): generator.cpp helper.hpp
	$(CXX) $(CXXFLAGS) generator.cpp -o $(TARGET)

# Clean rule to remove built files
clean:
	rm -f $(TARGET)

.PHONY: all clean
