# Makefile for compiling the snake game

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -lncurses

# Target executable
TARGET = snake

# Source files
SRC = snake.cpp

# Default rule
all: $(TARGET)

# Rule to compile the target
$(TARGET): $(SRC)
	$(CXX) -o $(TARGET) $(SRC) $(CXXFLAGS)

# Clean up
clean:
	rm -f $(TARGET)

