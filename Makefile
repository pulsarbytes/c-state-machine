# -*- Makefile -*-

CC=/usr/bin/gcc
COMPILER_FLAGS=-Wall -g

bin/c-state-machine: build/main.o
	$(CC) $(COMPILER_FLAGS) build/main.o -o bin/c-state-machine -lncurses

build/main.o: src/main.c
	$(CC) -c $(COMPILER_FLAGS) src/main.c -o build/main.o

clean:
	$(RM) build/* bin/*