CC=gcc
CFLAGS= -g -lSDL2 -Wall -DDEBUG_MEMORY
game-of-life: main.o board.o debug/debug.o
	$(CC) -o $@ main.o board.o debug/debug.o $(CFLAGS)


.PHONY: clean
clean:
	rm -f *.o game-of-life tests debug/*.o