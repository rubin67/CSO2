CC = clang
CFLAGS = -Wall -pedantic -std=c11 -fsanitize=address -g
LDFLAGS = -Wall -pedantic -fsanitize=address
LDLIBS = -lm


main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o $@
split.o: split.c split.h
	$(CC) $(CFLAGS) -c split.c -o $@

split: main.o split.o
	$(CC) $(CFLAGS) -o split main.o split.o


all: split
clean:
	rm --force split.o main.o