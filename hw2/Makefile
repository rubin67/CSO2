CC = clang
CFLAGS = -ftrivial-auto-var-init=pattern -D_XOPEN_SOURCE=700 -Og -g -std=c11 -Wall -pedantic -Wunused-result -O0 -fsanitize=address
LDFLAGS = -Wall -pedantic -fsanitize=address
LDLIBS = -lm

all: gettimings
gettimings.o: gettimings.c
	$(CC) $(CFLAGS) -c gettimings.c -o $@

gettimings: gettimings.o
	$(CC) $(CFLAGS) -o gettimings gettimings.o



clean:
	rm --force gettimings.o
