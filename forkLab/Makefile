CC = clang
CFLAGS = -ftrivial-auto-var-init=pattern -D_XOPEN_SOURCE=700 -Og -g -std=c11 -Wall -pedantic -Wunused-result -O0 -fsanitize=address
LDFLAGS = -Wall -pedantic -fsanitize=address
LDLIBS = -lm

all: my_system

my_system.o: my_system.c
	$(CC) $(CFLAGS) -c my_system.c -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o $@

my_system: my_system.o main.o
	$(CC) $(CFLAGS) -o my_system my_system.o main.o

clean:
	rm --force my_system.o
