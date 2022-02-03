#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define SCREEN_SIZE 800
#define N_SQUARES 10
#define SQUARE_SIZE (SCREEN_SIZE/N_SQUARES)

typedef struct SquareIndex {
    int x;
    int y;
} SquareIndex;

SquareIndex to_square_index(int x, int y) {

    SquareIndex index = (SquareIndex) {
        .x = x / SQUARE_SIZE,
        .y = y / SQUARE_SIZE
    };

    return index;
}

int main() {

    if (SDL_Init(SDL_INIT_VIDEO ) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Game of Life",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    SCREEN_SIZE, SCREEN_SIZE,
    0);
    
    if (!window) {
        SDL_Log("Unable to create SDL Window: %s", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        SDL_Log("Unable to create SDL Renderer: %s", SDL_GetError());
        return 1;
    }

    SDL_Rect squares[N_SQUARES * N_SQUARES];
    for (int i = 0; i < N_SQUARES; i++) {
        for (int j = 0; j < N_SQUARES; j++) {
            squares[i + N_SQUARES*j] = (SDL_Rect) {
                .h = SQUARE_SIZE,
                .w = SQUARE_SIZE,
                .x = SQUARE_SIZE*i,
                .y = SQUARE_SIZE*j
            };
        }
    }

    int colors[N_SQUARES * N_SQUARES];
    memset(&colors[0], 0, sizeof(int)*N_SQUARES * N_SQUARES);

    int back_board[N_SQUARES * N_SQUARES];
    int front_board[N_SQUARES * N_SQUARES];
    memset(&back_board[0], 0, sizeof(int)*N_SQUARES * N_SQUARES);
    memset(&front_board[0], 0, sizeof(int)*N_SQUARES * N_SQUARES);

    back_board[3 + N_SQUARES] = 1;
    back_board[4 + N_SQUARES] = 1;
    back_board[5 + N_SQUARES] = 1;

    clock_t start = clock();
    clock_t stop = clock();

    bool quit = false;
    bool paused = false;
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
                                printf("%d, %d\n", event.button.x, event.button.y);
                                SquareIndex index = to_square_index(event.button.x, event.button.y);
                                front_board[index.x + index.y * N_SQUARES] = 1;
                                printf("%d, %d\n", index.x, index.y);
                            }

                            if(event.button.button == SDL_BUTTON_RIGHT) {
                                SquareIndex index = to_square_index(event.button.x, event.button.y);
                                front_board[index.x + index.y * N_SQUARES] = 0;
                            }
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_SPACE:
                            paused = !paused;
                    }
                break;
            }
        }

        if (duration > 0.5) {

            for(int i = 0; i < N_SQUARES * N_SQUARES && !paused; i++) {
                
                int live_neighbours = 0;

                //No Wrapping
                //Your row
                live_neighbours += back_board[i - 1];
                live_neighbours += back_board[i + 1];

                //Upper row
                live_neighbours += back_board[ (i - 1) - N_SQUARES];
                live_neighbours += back_board[ (i + 1) - N_SQUARES];
                live_neighbours += back_board[ i - N_SQUARES];

                //Lower row
                live_neighbours += back_board[ (i -1) + N_SQUARES];
                live_neighbours += back_board[ (i + 1) + N_SQUARES];
                live_neighbours += back_board[ i + N_SQUARES];

                if(back_board[i]) {
                    front_board[i] = (int) (live_neighbours == 2 || live_neighbours == 3);
                } else {
                    front_board[i] = (int) (live_neighbours == 3);
                }
            }


            for(int i = 0; i < N_SQUARES * N_SQUARES; i++) {
                SDL_SetRenderDrawColor(renderer, 0xFF * front_board[i], 0, 0, 0);
                SDL_RenderFillRect(renderer, &squares[i]);
            }

            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderDrawRects(renderer, &squares[0], N_SQUARES * N_SQUARES);

            SDL_RenderPresent(renderer);

            memcpy(&back_board[0], &front_board[0], sizeof(int) * N_SQUARES * N_SQUARES);
            start = clock();
        }
        stop = clock();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}