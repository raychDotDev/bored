ifeq ($(OS),Windows_NT)
    UNAME = Windows
else
    UNAME := $(shell uname -s)
endif

CC = clang

CCFLAGS += -std=c2x
CCFLAGS += -Wall
# CCFLAGS += -Werror
CCFLAGS += -g
CCFLAGS += -O3
CCFLAGS += -Iinclude

# LDFLAGS += -lopenmp
ifeq ($(UNAME),Windows)
    CCFLAGS += -D_DEFAULT_SOURCE
    LDFLAGS += -lraylib.dll
    LDFLAGS += -lraylib
    LDFLAGS += -lwinmm -lgdi32 -lopengl32
    # LDFLAGS += -mwindows
    CCFLAGS += -static
    LDFLAGS += -static
    EXE_SUFFIX := .exe
else
    ifeq ($(UNAME),Linux)
        LDFLAGS += -lraylib -lm -ldl
    endif
endif

TARGET = Game$(EXE_SUFFIX)
SRC_DIR = src/
BIN_DIR = bin/
OBJ_DIR = obj/
ASSET_DIR = assets/

SOURCES := $(wildcard $(SRC_DIR)*.c $(SRC_DIR)**/*.c $(SRC_DIR)**/**/*.c )

ifeq ($(SOURCES),)
    SOURCES := $(wildcard $(SRC_DIR)*.c)
endif

OBJECTS := $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SOURCES))

.PHONY: all clean run rebuild

all: $(BIN_DIR)$(TARGET)

$(BIN_DIR)$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

run: all
	$(BIN_DIR)$(TARGET)

rebuild: clean all

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
