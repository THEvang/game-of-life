#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "memory.h"

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
    memcpy(&buffer[sizeof(int) * 2], board->cells, sizeof(int) * board->rows * board->columns);
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

void gol_save_to_file(Board* board, const char* filename) {

    FILE* f = fopen(filename, "w");
    if(f == NULL) {
        printf("failed to open file for writing\n");
    }

    unsigned char* buffer = malloc(sizeof(int) * 2 + sizeof(int) * board->rows * board->columns);
    gol_serialize_board(board, buffer);

    fwrite(buffer, 1, sizeof(int) + sizeof(int) * board->rows * board->columns, f);
    fclose(f);
    free(buffer);
}

void gol_load_from_file(Board* board, const char* filename) {

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("failed to open file for reading\n");
    }
    
    unsigned char* buffer = malloc(sizeof(int) * 2);
    fread(buffer, 1, sizeof(int) * 2, f);

    buffer = realloc(buffer, sizeof(int) * 2 + sizeof(int) * buffer[0] * buffer[sizeof(int)]);

    fread(&buffer[sizeof(int) * 2], 1, sizeof(int) * buffer[0] * buffer[sizeof(int)], f);

    gol_deserialize_board(board, buffer);
    
    fclose(f);
    free(buffer);
}

int gol_to_array_index(Board *b, int x, int y) {
    return x + y * b->columns;
}

void gol_activate_cell(Board* b, int x, int y) {
    b->cells[gol_to_array_index(b, x, y)] = 1;
}

void gol_kill_cell(Board* b, int x, int y) {
    b->cells[gol_to_array_index(b, x, y)] = 0;
}

bool gol_is_cell_alive(Board* b, int x, int y) {
    return b->cells[gol_to_array_index(b, x, y)];
}