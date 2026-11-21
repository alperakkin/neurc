COMPILER=gcc
FLAGS=-Wall -Wextra -O2
INCLUDES= -Iinclude
BUILD_FOLDER=build
SRC=src
all: $(BUILD_FOLDER)/neurc

clean:
	rm -rf build

$(BUILD_FOLDER)/neurc:$(SRC)/main.c
	mkdir -p $(BUILD_FOLDER)
	$(COMPILER) $(FLAGS) $(INCLUDES) $(SRC)/main.c  -o $(BUILD_FOLDER)/neurc

run:
	./build/neurc





.PHONY: all clean run