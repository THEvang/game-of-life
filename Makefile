CC=gcc
CFLAGS= -lSDL2 -Wall
game-of-life: main.o
	$(CC) -o $@ main.o $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o game-of-life