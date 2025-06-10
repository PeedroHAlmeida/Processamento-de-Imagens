# Makefile for the PGH decoder project

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99

# Target executable
TARGET = decode

# Source files
SOURCES = decode.c huffman.c imagelib.c

# Object files derived from sources
OBJECTS = $(SOURCES:.c=.o)

# Default target: build the executable
all: $(TARGET)

# Rule to link object files into the final executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to compile a .c source file into a .o object file
%.o: %.c *.h
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up the directory
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean