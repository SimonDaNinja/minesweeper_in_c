.PHONY: all

CC = gcc

OBJS = minesweeper.o

CFLAGS = -O3 -Wall

all: minesweeper

minesweeper: minesweeper.o
	$(CC) $(CFLAGS) -o $@ minesweeper.o -march=native

minesweeper.o: minesweeper.c
	$(CC) -c minesweeper.c

.PHONY: clean

clean:
	rm -f $(OBJS) minesweeper
