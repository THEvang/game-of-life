#pragma once

#include <stdbool.h>

typedef struct Board {
    int rows;
    int columns;
    int* cells;
} Board;

Board* gol_init_board(int rows, int columns);
void gol_destroy_board(Board* b);
bool gol_is_cell_alive(Board* b, int x, int y);
void gol_activate_cell(Board* b, int x, int y);
void gol_kill_cell(Board* b, int x, int y);

void gol_tick_board(Board *new_board, Board *old_board);

void gol_serialize_board(Board* board, unsigned char* buffer);
void gol_deserialize_board(Board* board, unsigned char* data);

void gol_save_to_file(Board* board, const char* filename);
void gol_load_from_file(Board* board, const char* filename);