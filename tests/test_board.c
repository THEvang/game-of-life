#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "board.h"
#include "../memory.h"

#define BEGIN_TEST(name) printf("TEST CASE: %s\n", name); {

#define END_TEST() } printf("PASSED\n");

int main() {

    BEGIN_TEST("Board is initialized correctly")
        Board* b = gol_init_board(10, 20);
        assert(b != NULL);
        assert(b->rows == 10);
        assert(b->columns == 20);
        assert(b->cells != NULL);
        gol_destroy_board(b);
    END_TEST()

    BEGIN_TEST("Destroy board")
        Board* b = gol_init_board(10, 20);
        gol_destroy_board(b);
    END_TEST()

    BEGIN_TEST("Cells can created")
        Board* b = gol_init_board(10, 20);
        assert(!gol_is_cell_alive(b, 1, 1));
        gol_activate_cell(b, 1,1);
        assert(gol_is_cell_alive(b, 1, 1));
        gol_destroy_board(b);
    END_TEST()

    BEGIN_TEST("Cells can be destroyed")
        Board* b = gol_init_board(10, 20);
        assert(!gol_is_cell_alive(b, 1, 1));
        gol_activate_cell(b, 1,1);
        assert(gol_is_cell_alive(b, 1, 1));
        gol_kill_cell(b, 1, 1);
        assert(!gol_is_cell_alive(b, 1, 1));
        gol_destroy_board(b);
    END_TEST()

    PRINT_DEBUG_INFO()

    return 1;
}