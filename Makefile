ifeq ($(OS),Windows_NT)
	UNAME = Windows
else
	UNAME := $(shell uname -s)
endif

CC = gcc

CCFLAGS += -std=c2x
CCFLAGS += -Wall
CCFLAGS += -g
CCFLAGS += -static
LDFLAGS += -static-libgcc
CCFLAGS += -Iinclude

ifeq ($(UNAME),Windows)
	LDFLAGS += -static
	LDFLAGS += -lraylib
	LDFLAGS += -mwindows
	LDFLAGS += -lwinmm 
	LDFLAGS += -lgdi32 
	LDFLAGS += -lopengl32
	EXE_SUFFIX := .exe
else ifeq ($(UNAME),Linux)
	LDFLAGS += -lraylib -lm -ldl
endif

TARGET = bored$(EXE_SUFFIX)
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
	cp -r $(ASSET_DIR) $(BIN_DIR)

$(BIN_DIR)$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CCFLAGS)

$(OBJ_DIR):
	mkdir -p $@

run: all
	$(BIN_DIR)$(TARGET)

rebuild: clean all

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
