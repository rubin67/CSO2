CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -fsanitize=address
LDFLAGS = -Wall -pedantic -fsanitize=address
LDLIBS = -lm

all: pt

pt.o: pt.c config.h mlpt.h
	$(CC) $(CFLAGS) $(LDLIBS) -c pt.c -o $@

pt: libmlpt.a pt.o
	$(CC) $(CFLAGS) $(LDLIBS) -o pt pt.o libmlpt.a

libmlpt.a: pt.o mlpt.h
	ar -rcs libmlpt.a pt.o
clean:
	rm --force pt.o libmlpt.a
