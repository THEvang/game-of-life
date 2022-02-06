CC=gcc
CFLAGS= -g -lSDL2 -Wall
game-of-life: main.o board.o
	$(CC) -o $@ main.o board.o $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o game-of-life