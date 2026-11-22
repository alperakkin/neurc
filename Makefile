CC        = gcc
CFLAGS    = -Wall -Wextra -O2 -Iinclude -MMD -MP
LDFLAGS   =
LDLIBS    = -lm

DEBUG_FLAGS = -Wall -Wextra -g -O1 -fsanitize=address -fno-omit-frame-pointer -Iinclude -MMD -MP

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
	$(CC) $(LDFLAGS) $< $(LIB) $(LDLIBS) -o $@

$(LIB): $(OBJS)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

run: $(BIN)
	./$(BIN)

# Debug hedefleri
debug: CFLAGS  = $(DEBUG_FLAGS)
debug: LDFLAGS = -fsanitize=address
debug: clean all

debug-run: debug
	./$(BIN)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)

.PHONY: all run debug debug-run clean