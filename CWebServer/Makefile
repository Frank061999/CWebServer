# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Directories
SRC_DIR = .
DATASTRUCTURES_DIR = DataStructures

# Source files
SRC_FILES = $(SRC_DIR)/main.c $(SRC_DIR)/http.c $(DATASTRUCTURES_DIR)/HashMap.c

# Object files
OBJ_FILES = $(SRC_FILES:.c=.o)

# Executable name
EXEC = CWebServer

# Default target
all: $(EXEC)

# Linking the executable
$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ_FILES)

# Compiling source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJ_FILES)
