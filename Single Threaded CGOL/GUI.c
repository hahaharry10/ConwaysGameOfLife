#include "GUI.h"
#include "SDL_render.h"
#include "SDL_video.h"

int INIT_GRAPHICS(SDL_Window** window, SDL_Renderer** renderer, SDL_Rect** cells) {
    int i;

    if( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
        fprintf(stderr, "ERROR: [ SDL_Init() ] %s\n", SDL_GetError());
        return 1;
    }

    *window = SDL_CreateWindow("Conways Game of Life", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
    if( !*window ) {
        fprintf(stderr, "ERROR: [ !window ] %s\n", SDL_GetError());
        return 1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if( !*renderer ) {
        fprintf(stderr, "ERROR: [ !renderer ] %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        return 1;
    }

    *cells = (SDL_Rect *) calloc(CELL_GRID_WIDTH*CELL_GRID_HEIGHT, sizeof(SDL_Rect));
    if( !cells ) {
        fprintf(stderr, "ERROR: [ !cells ] Failed to allocate memory to `cells`.\n");
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        return 1;
    }
    for( i = 0; i < CELL_GRID_HEIGHT*CELL_GRID_WIDTH; i++ ) {
        (*cells)[i].x = 200 + ((i % CELL_GRID_WIDTH) * CELL_WIDTH);
        (*cells)[i].y = 200 + ((i / CELL_GRID_WIDTH) * CELL_HEIGHT);
        (*cells)[i].w = CELL_WIDTH;
        (*cells)[i].h = CELL_HEIGHT;
    }

    return 0;
}

void RENDER_CELLS( char** cellGrid, SDL_Renderer* renderer, SDL_Rect* cells ) {
    int i;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for( i = 0; i < CELL_GRID_WIDTH*CELL_GRID_HEIGHT; i++ ) {
/*         fprintf(stdout, "Square %i:\t%i\t%i\t%i\t%i\n", i, cells[i].x, cells[i].y, cells[i].w, cells[i].h); */
        SDL_RenderDrawRect(renderer, cells+i);
        if( cellGrid[i/CELL_GRID_WIDTH][i%CELL_GRID_WIDTH] )
            SDL_RenderFillRect(renderer, cells+i);
    }
    SDL_RenderPresent(renderer);
}

void QUIT_GRAPHICS( SDL_Window* window, SDL_Renderer* renderer, SDL_Rect* cells ) {
    free(cells);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
