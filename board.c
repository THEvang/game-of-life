#include <string.h>
#include <stdlib.h>
#include "board.h"

Board* gol_init_board(int rows, int columns) {

    Board* b = malloc(sizeof(Board));
    b->rows = rows;
    b->columns = columns;
    b->cells = malloc(sizeof(int) * rows * columns);
    memset(b->cells, 0, sizeof(int) * rows * columns);
    return b;
}

void gol_destroy_board(Board* b) {
    free(b->cells);
    free(b);
}

void gol_serialize_board(Board* board, unsigned char* buffer) {
    memcpy(buffer, &board->rows, sizeof(int));
    memcpy(&buffer[sizeof(int)], &board->columns, sizeof(int));
    memcpy(&buffer[sizeof(int)*2], &board->cells, sizeof(int) * board->rows * board->columns);
}

void gol_deserialize_board(Board* board, unsigned char* data) {
    memcpy(&board->rows, data, sizeof(int));
    memcpy(&board->columns, &data[sizeof(int)], sizeof(int));
    memcpy(board->cells, &data[sizeof(int)*2], sizeof(int) * board->rows * board->columns);
}

void gol_tick_board(Board *new_board, Board *old_board) {

    int rows = old_board->rows;
    int columns = old_board->columns;
       
    for(int i = 0; i < rows * columns; i++) {
        
        int live_neighbours = 0;

        int row = i / columns;

        int* previous_row = &old_board->cells[((row + rows - 1) % rows) * columns];
        int* current_row = &old_board->cells[row * columns];
        int* next_row = &old_board->cells[((row + 1) % rows) * columns];

        int west = (i + columns - 1) % columns;
        int east = (i + 1) % columns;

        //Your row
        live_neighbours += current_row[west];
        live_neighbours += current_row[east];

        //Upper row
        live_neighbours += next_row[west];
        live_neighbours += next_row[i % columns];
        live_neighbours += next_row[east];

        //Lower row
        live_neighbours += previous_row[west];
        live_neighbours += previous_row[i % columns];
        live_neighbours += previous_row[east];

        if(old_board->cells[i]) {
            new_board->cells[i] = (int) (live_neighbours == 2 || live_neighbours == 3);
        } else {
            new_board->cells[i] = (int) (live_neighbours == 3);
        }
    }

    memcpy(old_board->cells, new_board->cells, sizeof(int) * rows * columns);
}