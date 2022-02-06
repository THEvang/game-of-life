#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "board.h"

typedef struct SquareIndex {
    int x;
    int y;
} SquareIndex;

typedef struct Renderer {
    SDL_Window* window;
    SDL_Renderer* renderer;
} Renderer;

Renderer* init_renderer(int width, int height) {

    if (SDL_Init(SDL_INIT_VIDEO ) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return NULL;
    }
    
    SDL_Window* window = SDL_CreateWindow("Game of Life",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    width, height,
    0);
    
    if (!window) {
        SDL_Log("Unable to create SDL Window: %s", SDL_GetError());
        return NULL;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        SDL_Log("Unable to create SDL Renderer: %s", SDL_GetError());
        return NULL;
    }

    Renderer* r = malloc(sizeof(Renderer));
    r->renderer = renderer;
    r->window = window;

    return r;
}

void tick_board(int *new_board, int *old_board);
void render_board(Board* board, Renderer* renderer, SDL_Rect* squares, bool draw_grid);

SquareIndex to_square_index(int x, int y, int square_size) {
    SquareIndex index = (SquareIndex) {
        .x = x / square_size,
        .y = y / square_size
    };

    return index;
}

void usage() {
    printf("USAGE: game-of-life [OPTION] N M\n");
    printf("Conway's game of life\n");
}

int main(int argc, char* argv[]) {

    char* options = "hn:";
    int option;
    while((option = getopt(argc, argv, options)) != -1) {
        switch(option) {
        case 'h':
            usage();
            return 0;
        case '?':
            printf("Unknown option: %c\n", option);
            return 1;
        }
    }

    if (optind == argc) {
        usage();
        return 1;
    }

    int columns = atoi(argv[optind++]);
    int rows = atoi(argv[optind++]);

    if (columns <= 0 || rows <= 0) {
        printf("Rows and columns cannot be zero or negative\n");
        return 1;
    }

    int height = rows * 10;
    int width = columns * 10;
    int square_size = height / rows; 

    Renderer* renderer = init_renderer(width, height);
    if (!renderer) {
        printf("Unable to start renderer\n");
        return 1;
    } 

    SDL_Rect *squares = malloc(sizeof(SDL_Rect) * rows * columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            squares[j + columns*i] = (SDL_Rect) {
                .h = square_size,
                .w = square_size,
                .x = square_size*j,
                .y = square_size*i
            };
        }
    }

    int *colors = malloc(sizeof(int) * rows * columns);
    memset(colors, 0, sizeof(int) * rows * columns);

    Board* back_board = gol_init_board(rows, columns);
    Board* front_board = gol_init_board(rows, columns);

    back_board->cells[1 + columns] = 1;
    back_board->cells[1 + 2 * columns] = 1;
    back_board->cells[1 + 3 * columns] = 1;

    clock_t start = clock();
    clock_t stop = clock();

    bool quit = false;
    bool paused = false;
    bool draw_grid = true;
    while (!quit) {

        double duration =  (double) (stop - start) / CLOCKS_PER_SEC;

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch (event.button.type) {
                        case SDL_MOUSEBUTTONDOWN:
                            if(event.button.button == SDL_BUTTON_LEFT) {
                                SquareIndex index = to_square_index(event.button.x, event.button.y, square_size);
                                back_board->cells[index.x + index.y * columns] = 1;
                                front_board->cells[index.x + index.y * columns] = 1;
                                printf("%d, %d\n", index.x, index.y);
                            }

                            if(event.button.button == SDL_BUTTON_RIGHT) {
                                SquareIndex index = to_square_index(event.button.x, event.button.y, square_size);
                                back_board->cells[index.x + index.y * columns] = 0;
                                front_board->cells[index.x + index.y * columns] = 0;
                            }
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_SPACE:
                            paused = !paused;
                            break;
                        case SDLK_s: {
                            unsigned char* buffer = malloc(sizeof(int) * 2 + sizeof(int) * rows * columns);
                            gol_serialize_board(front_board, buffer);
                            FILE* f = fopen("board.bd", "w");
                            if(f == NULL) {
                                printf("failed to open file for writing\n");
                            }
                            fwrite(buffer, 1, sizeof(int) + sizeof(int) * rows * columns, f);
                            fclose(f);
                            free(buffer);
                        }
                        break;
                        case SDLK_l: {
                            FILE* f = fopen("board.bd", "r");
                            if (f == NULL) {
                                printf("failed to open file for reading\n");
                            }
                            unsigned char* buffer = malloc(sizeof(int) * 2 + sizeof(int) * rows * columns);
                            fread(buffer, 1, sizeof(int) * 2 + sizeof(int) * rows * columns, f);
                            gol_deserialize_board(back_board, buffer);
                            gol_deserialize_board(front_board, buffer);
                            fclose(f);
                            free(buffer);
                        }
                        case SDLK_n: {
                            draw_grid = !draw_grid;
                        }
                        break;
                        
                    }
                break;
            }
        }

        if (duration > 0.5 && !paused) {
            gol_tick_board(front_board, back_board);
            start = clock();
        }

        render_board(front_board, renderer, &squares[0], draw_grid);

        stop = clock();
    }

    SDL_DestroyWindow(renderer->window);
    SDL_Quit();

    free(renderer);
    gol_destroy_board(front_board);
    gol_destroy_board(back_board);

    return 0;
}

void render_board(Board* board, Renderer* r, SDL_Rect* squares, bool draw_grid) {

    for(int i = 0; i < board->rows * board->columns; i++) {
        SDL_SetRenderDrawColor(r->renderer, 0xFF * board->cells[i], 0, 0, 0);
        SDL_RenderFillRect(r->renderer, &squares[i]);
    }

    if(draw_grid) {
        SDL_SetRenderDrawColor(r->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawRects(r->renderer, &squares[0], board->rows * board->columns);
    }

    SDL_RenderPresent(r->renderer);
}