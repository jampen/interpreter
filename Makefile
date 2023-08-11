# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Directories
SRC_DIR = app
OBJ_DIR = obj
BIN_DIR = bin
DATA_DIR = data

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Executable
TARGET = $(BIN_DIR)/app

# Phony targets
.PHONY: all clean

all: $(TARGET) copy_data

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

copy_data:
	@mkdir -p $(BIN_DIR)
	cp -r $(DATA_DIR) $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run:
	bin/app

# Dependencies
DEPS = $(SRCS:.c=.d)
-include $(DEPS)
