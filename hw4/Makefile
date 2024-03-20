CC = clang
CFLAGS = -ftrivial-auto-var-init=pattern -D_XOPEN_SOURCE=700 -Og -g -std=c11 -Wall -pedantic -Wunused-result -O0 -fsanitize=address
LDFLAGS = -Wall -pedantic -fsanitize=address
LDLIBS = -lm

all: pt

pt.o: pt.c config.h mlpt.h
	$(CC) $(CFLAGS) -c pt.c -o libmlpt.a

pt: libmlpt.a pt.o
	$(CC) $(CFLAGS) -o pt pt.o libmlpt.a

libmlpt.a: pt.o mlpt.h
	ar -rcs libmlpt.a pt.o mlpt.h

clean:
	rm --force pt.o libmlpt.a
