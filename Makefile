CC        = gcc
CFLAGS    = -Wall -Wextra -O2 -Iinclude -MMD -MP
LDLIBS    = -lm

SRC_DIR   = src
BUILD_DIR = build

MAIN  = $(SRC_DIR)/main.c
SRCS  = $(filter-out $(MAIN),$(wildcard $(SRC_DIR)/*.c))
OBJS  = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS  = $(OBJS:.o=.d) $(BUILD_DIR)/main.d
LIB   = $(BUILD_DIR)/libneurc.a
BIN   = $(BUILD_DIR)/neurc

all: $(BIN)

$(BIN): $(BUILD_DIR)/main.o $(LIB)
	$(CC) $< $(LIB) $(LDLIBS) -o $@

$(LIB): $(OBJS)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

run: $(BIN)
	./$(BIN)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)

.PHONY: all run clean