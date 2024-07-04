# Compiler and flags
CC = gcc
CFLAGS = -g --debug -I/opt/homebrew/include -I./lib -I./src/include 
LDFLAGS = -L/opt/homebrew/lib -lglfw -framework OpenGL -ldl -lcglm -lm

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Files
SOURCES := $(shell find $(SRC_DIR) -name '*.c')
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/minecraft

# Phony targets
.PHONY: all clean run update_compile_commands

# Default target
all: update_compile_commands $(TARGET)

# Update compile_commands.json
update_compile_commands:
	@bear -- make clean
	@bear -- make $(TARGET)

# Linking the target
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compiling source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Cleaning up build files
clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

# Build and run the program
run: all
	./$(TARGET)

