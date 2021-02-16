clean:
	rm -rf octo

build:
	gcc -std=c99 -o octo src/*.c

all: clean build
