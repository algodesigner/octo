CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Isrc
DEPFLAGS = -MMD -MP

OBJ_DIR = obj
SRC_DIR = src
TST_DIR = tst

# Main binary
TARGET = octo
# Test runner binary
TEST_TARGET = test_runner

# Source files
CORE_SRCS = $(filter-out $(SRC_DIR)/octo.c, $(wildcard $(SRC_DIR)/*.c))
OCTO_SRC = $(SRC_DIR)/octo.c
TEST_SRCS = $(wildcard $(TST_DIR)/*.c)

# Object files
CORE_OBJS = $(CORE_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OCTO_OBJ = $(OCTO_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_OBJS = $(TEST_SRCS:$(TST_DIR)/%.c=$(OBJ_DIR)/%.o)

# Dependency files
DEPS = $(CORE_OBJS:.o=.d) $(OCTO_OBJ:.o=.d) $(TEST_OBJS:.o=.d)

.PHONY: all clean test rebuild

all: $(TARGET)

$(TARGET): $(OCTO_OBJ) $(CORE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS) $(CORE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(TST_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_TARGET)

rebuild: clean all

-include $(DEPS)
