CC=gcc
CFLAGS=-I.
BUILD_DIR=./build/
FLAGS=-lncurses

.PHONY: clean build

build: main.c
	$(CC) -o $(BUILD_DIR)cchess.out main.c $(FLAGS)

clean:
	rm -rf ./build
	mkdir ./build
