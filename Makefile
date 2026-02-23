SRCS = $(filter-out src/octo.c, $(wildcard src/*.c))
TEST_SRCS = $(wildcard tst/*.c)

clean:
	rm -rf octo test_runner

build:
	gcc -std=c99 -o octo src/*.c

test: clean
	gcc -std=c99 -Isrc -o test_runner $(SRCS) $(TEST_SRCS)
	./test_runner

all: clean build
