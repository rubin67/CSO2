CC = clang
CFLAGS = -ftrivial-auto-var-init=pattern -D_XOPEN_SOURCE=700 -Og -g -std=c11 -Wall -pedantic -Wunused-result -O0 -fsanitize=address
LDFLAGS = -Wall -pedantic -fsanitize=address
LDLIBS = -lm


main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o $@
split.o: split.c split.h
	$(CC) $(CFLAGS) -c split.c -o $@

split: main.o split.o split.h
	$(CC) $(CFLAGS) -o split main.o split.o


all: split
clean:
	rm --force split.o main.o
