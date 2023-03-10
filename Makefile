CC=gcc
CFLAGS=-I.
BUILD_DIR=./build
SRC_DIR=./src
FLAGS=-lncursesw -lcurl -lcjson -DUSE_SYMBOLS

.PHONY: clean build

build: $(SRC_DIR)/* $(SRC_DIR)/menu/* $(SRC_DIR)/game/*
	$(CC) -o $(BUILD_DIR)/cchess.out $(SRC_DIR)/*.c $(SRC_DIR)/menu/*.c  $(SRC_DIR)/game/*.c $(FLAGS)

clean:
	rm -rf ./build
	mkdir ./build
