CC=gcc
CFLAGS=-I.
BUILD_DIR=./build
SRC_DIR=./src
FLAGS=-lncursesw -DUSE_FIGURES=1

.PHONY: clean build

build: $(SRC_DIR)/*.c
	$(CC) -o $(BUILD_DIR)/cchess.out $(SRC_DIR)/*.c $(FLAGS)

clean:
	rm -rf ./build
	mkdir ./build
