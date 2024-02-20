CC = clang
CFLAGS = -ftrivial-auto-var-init=pattern -D_XOPEN_SOURCE=700 -Og -g -std=c11 -Wall -pedantic -Wunused-result -O0 -fsanitize=address
LDFLAGS = -Wall -pedantic -fsanitize=address
LDLIBS = -lm

all: fork_run
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o $@

fork_run.o: fork_run.c
	$(CC) $(CFLAGS) -c fork_run.c -o $@


fork_run: fork_run.o main.o
	$(CC) $(CFLAGS) -o fork_run fork_run.o main.o



clean:
	rm --force fork_run.o main.o
