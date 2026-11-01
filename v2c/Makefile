
# Variables for compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O1
TARGET = solverv2c

# Default rule (runs when you type 'make')
all: $(TARGET)

# Rule to build the executable
$(TARGET): helperv2c.hpp helperv2c.cpp solverv2c.cpp ../helper.hpp
	$(CXX) $(CXXFLAGS) solverv2c.cpp helperv2c.cpp -o $(TARGET)

# Clean rule to remove built files
clean:
	rm -f $(TARGET)

.PHONY: all clean
