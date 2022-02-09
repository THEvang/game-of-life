#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "board.h"
#include "../memory.h"

#define BEGIN_TEST() {

#define END_TEST() }

int main() {

    BEGIN_TEST()
        Board* b = gol_init_board(10, 20);
        assert(b != NULL);
        assert(b->rows == 10);
        assert(b->columns == 20);
        assert(b->cells != NULL);
        gol_destroy_board(b);
    END_TEST()

    BEGIN_TEST()
        Board* b = gol_init_board(10, 20);
        gol_destroy_board(b);
    END_TEST()

    PRINT_DEBUG_INFO()

    return 1;
}