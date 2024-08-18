# Directories
SRC_DIR = src
BACK_DIR = back
BIN_DIR = bin

# Compiler and flags
CC = g++
CFLAGS = -I $(SRC_DIR)/include -L $(SRC_DIR)/lib
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

# Output target
TARGET = $(BIN_DIR)/run.exe

# Source files
SRCS = main.cpp $(BACK_DIR)/gamestate.cpp $(BACK_DIR)/hash.cpp $(BACK_DIR)/Entity.cpp

# Build target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(LDFLAGS)

# Clean up
clean:
	del /F /Q $(BIN_DIR)\run.exe

# Phony targets
.PHONY: all clean
