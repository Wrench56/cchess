CC=gcc
CFLAGS=-I.
BUILD_DIR=./build
SRC_DIR=./src
FLAGS=-lncurses

.PHONY: clean build

build: $(SRC_DIR)/main.c
	$(CC) -o $(BUILD_DIR)/cchess.out $(SRC_DIR)/main.c $(FLAGS)

clean:
	rm -rf ./build
	mkdir ./build
