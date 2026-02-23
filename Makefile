CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Iinclude
DEPFLAGS = -MMD -MP

BIN_DIR = bin
BUILD_DIR = build
SRC_DIR = src
INC_DIR = include/octo
TST_DIR = tests

# Main binary
TARGET = $(BIN_DIR)/octo
# Test runner binary
TEST_TARGET = $(BIN_DIR)/test_runner

# Source files
CORE_SRCS = $(filter-out $(SRC_DIR)/octo.c, $(wildcard $(SRC_DIR)/*.c))
OCTO_SRC = $(SRC_DIR)/octo.c
TEST_SRCS = $(wildcard $(TST_DIR)/*.c)

# Object files
CORE_OBJS = $(CORE_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OCTO_OBJ = $(OCTO_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_OBJS = $(TEST_SRCS:$(TST_DIR)/%.c=$(BUILD_DIR)/%.o)

# Dependency files
DEPS = $(CORE_OBJS:.o=.d) $(OCTO_OBJ:.o=.d) $(TEST_OBJS:.o=.d)

.PHONY: all clean test rebuild

all: $(TARGET)

$(TARGET): $(OCTO_OBJ) $(CORE_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS) $(CORE_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(TST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(BIN_DIR) $(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

rebuild: clean all

-include $(DEPS)
