# Directories
SRC_DIR = src
BACK_DIR = back
BIN_DIR = bin

# Compiler and flags
CC = g++
CFLAGS = -I $(SRC_DIR)/include -I $(BACK_DIR) -L $(SRC_DIR)/lib -include $(BACK_DIR)/global.h
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer

# Output target
TARGET = $(BIN_DIR)/run.exe

# Source files
SRCS = main.cpp $(BACK_DIR)/gamestate.cpp $(BACK_DIR)/window.cpp $(BACK_DIR)/events.cpp

# Build target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(LDFLAGS)

# Clean up
clean:
	del /F /Q $(BIN_DIR)\run.exe

# Phony targets
.PHONY: all clean
