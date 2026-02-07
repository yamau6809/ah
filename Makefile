# ---------------------------------------------------------
#  Makefile for ah
# ---------------------------------------------------------
CC	 := gcc
WARNINGS := -Wall -Wextra

# Build types
DEBUG_FLAGS   := -g -O0 -DDEBUG
RELEASE_FLAGS := -O2 -DNDEBUG

# Directories
SRC_DIR	 := src
BUILD_DIR := build
BIN_DIR	  := bin

# Sources and objects
SRC := $(wildcard $(SRC_DIR)/*.c)

# Pattern for object files per config:
#   build/<config>/file.o
OBJ_DIR = $(BUILD_DIR)/$(CONFIG)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target name
TARGET := $(BIN_DIR)/ah

# ---------------------------------------------------------
#  Top-level targets
# ---------------------------------------------------------
.PHONY: all debug release clean run

all: debug

debug:
	@$(MAKE) CONFIG=debug CFLAGS="$(WARNINGS) $(DEBUG_FLAGS)" $(TARGET)

release:
	@$(MAKE) CONFIG=release CFLAGS="$(WARNINGS) $(RELEASE_FLAGS)" $(TARGET)

# ---------------------------------------------------------
#  Build rules
# ---------------------------------------------------------
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ---------------------------------------------------------
#  Utility targets
# ---------------------------------------------------------
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

run: debug
	./$(TARGET)
