# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm
SRC_DIR = ../data-structures
BUILD_DIR = ./build
TARGET = $(BUILD_DIR)/test
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
FILE_SRC = day13.c
FILE_OBJ = $(BUILD_DIR)/day13.o

# Rules
all: $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile test.c
$(FILE_OBJ): $(FILE_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile source files from ../../data-structures
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link all object files into the final executable
$(TARGET): $(FILE_OBJ) $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)

# Run the program
run: all
	./$(TARGET)

.PHONY: all clean
