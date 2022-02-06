#pragma once

typedef struct Board {
    int rows;
    int columns;
    int* cells;
} Board;

Board* gol_init_board(int rows, int columns);
void gol_destroy_board(Board* b);

void gol_tick_board(Board *new_board, Board *old_board);
void gol_serialize_board(Board* board, unsigned char* buffer);
void gol_deserialize_board(Board* board, unsigned char* data);