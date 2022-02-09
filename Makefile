CC=gcc
CFLAGS= -g -lSDL2 -Wall -DDEBUG_MEMORY -I.
game-of-life: main.o board.o debug/debug.o
	$(CC) -o $@ main.o board.o debug/debug.o $(CFLAGS)

test_board: board.o tests/test_board.o debug/debug.o
	$(CC) -o $@ tests/test_board.o board.o debug/debug.o  ${CFLAGS}

.PHONY: clean
clean:
	rm -f *.o game-of-life debug/*.o tests/*.o