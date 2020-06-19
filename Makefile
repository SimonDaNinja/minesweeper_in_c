.PHONY: all

CC = gcc

OBJS = minesweeper.o

all: minesweeper

minesweeper: minesweeper.o
	$(CC) -o $@ minesweeper.o

minesweeper.o: minesweeper.c
	$(CC) -c minesweeper.c

.PHONY: clean

clean:
	rm -f $(OBJS) minesweeper
