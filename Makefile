.PHONY: all

CC = gcc

OBJS = main.o

all: main

main: main.o
	$(CC) -o $@ main.o

main.o: main.c
	$(CC) -c main.c

.PHONY: clean

clean:
	rm -f $(OBJS) main
