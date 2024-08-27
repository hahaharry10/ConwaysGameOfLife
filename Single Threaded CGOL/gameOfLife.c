#include "gameOfLife.h"
#include "GUI.h"
#include "SDL_events.h"
#include <stdio.h>
#include <stdlib.h>


void runSimulation(char** cellGrid) {
    int r, c; /* Using r and c as iterators to represent row and columns on the grid. */
    char** neighbourGrid;

    /* Create grid that contains the number of neighbours of each cell: */
    neighbourGrid = (char **) calloc(CELL_GRID_HEIGHT, sizeof(char *));
    for( r = 0; r < CELL_GRID_HEIGHT; r++ ) {
        neighbourGrid[r] = (char *) calloc(CELL_GRID_WIDTH, sizeof(char));
        for( c = 0; c < CELL_GRID_WIDTH; c++ )
            neighbourGrid[r][c] = 0;
    }

    /* Get neighbour count for all cells: */
    for( r = 0; r < CELL_GRID_WIDTH; r++ )
        for( c = 0; c < CELL_GRID_HEIGHT; c++ ) {
            if( cellGrid[r][c] ) {
                if( r != 0 ) {
                    ++neighbourGrid[r-1][c];                                     /* Immediate Above */
                    if( c != 0 ) { ++neighbourGrid[r-1][c-1]; }                  /* Top Left */
                    if( c != CELL_GRID_WIDTH-1 ) { ++neighbourGrid[r-1][c+1]; }  /* Top Right */
                }
                if( r != CELL_GRID_HEIGHT ) {
                    ++neighbourGrid[r+1][c];                                     /* Immediate Below */
                    if( c != 0 ) { ++neighbourGrid[r+1][c-1]; }                  /* Lower Left */
                    if( c != CELL_GRID_HEIGHT-1 ) { ++neighbourGrid[r+1][c+1]; } /* Lower Right */
                }
                if( c != 0 ) { ++neighbourGrid[r][c-1]; }                        /* Immediate Left */
                if( c != CELL_GRID_WIDTH ) { ++neighbourGrid[r][c+1]; }          /* Immediate Right */
            }
        }

    /* Update cellGrid: */
    for( r = 0; r < CELL_GRID_HEIGHT; r++ )
        for( c = 0; c < CELL_GRID_WIDTH; c++ ) {
            if( cellGrid[r][c] && (neighbourGrid[r][c] < 2 || neighbourGrid[r][c] > 3) ) { cellGrid[r][c] = 0; }
            if( !cellGrid[r][c] && neighbourGrid[r][c] == 3 ) { cellGrid[r][c] = 1; }
        }
}

char** initCellGrid(void) {
    int i, j;
    char** cellGrid; /* Char used to allocate 8 bits per cell */

    cellGrid = (char **) calloc(CELL_GRID_HEIGHT, sizeof(char *));
    for( i = 0; i < CELL_GRID_HEIGHT; i++ ) {
        cellGrid[i] = (char *) calloc(CELL_GRID_WIDTH, sizeof(char));
        for( j = 0; j < CELL_GRID_WIDTH; j++ )
            cellGrid[i][j] = 0;
    }

    return cellGrid;
}

void freeCellGrid(char** cellGrid) {
    int i;

    for( i = 0; i < CELL_GRID_HEIGHT; i++ ) 
        free(cellGrid[i]);
    free(cellGrid);
}

void printCellGrid(char** cellGrid) {
    int i, j;

    for( i = 0; i < CELL_GRID_HEIGHT; i++ ) {
        for( j = 0; j < CELL_GRID_WIDTH; j++ )
            printf("%i ", cellGrid[i][j]);
        printf("\n");
    }
}

void createToad( char** cellGrid, int strt_r, int strt_c ) {
    cellGrid[strt_r+1][strt_c+0] = 1;
    cellGrid[strt_r+1][strt_c+1] = 1;
    cellGrid[strt_r+1][strt_c+2] = 1;
    cellGrid[strt_r+0][strt_c+1] = 1;
    cellGrid[strt_r+0][strt_c+2] = 1;
    cellGrid[strt_r+0][strt_c+3] = 1;
}

void createBeacon( char** cellGrid, int strt_r, int strt_c ) {
    cellGrid[strt_r+0][strt_c+0] = 1;
    cellGrid[strt_r+1][strt_c+0] = 1;
    cellGrid[strt_r+0][strt_c+1] = 1;
    cellGrid[strt_r+3][strt_c+2] = 1;
    cellGrid[strt_r+3][strt_c+3] = 1;
    cellGrid[strt_r+2][strt_c+3] = 1;
}

void createBlock( char** cellGrid, int strt_r, int strt_c ) {
    cellGrid[strt_r+0][strt_c+0] = 1;
    cellGrid[strt_r+1][strt_c+1] = 1;
    cellGrid[strt_r+1][strt_c+0] = 1;
    cellGrid[strt_r+1][strt_c+1] = 1;
}

void createGlider( char** cellGrid, int strt_r, int strt_c ) {
    cellGrid[strt_r+0][strt_c+2] = 1;
    cellGrid[strt_r+1][strt_c+2] = 1;
    cellGrid[strt_r+2][strt_c+2] = 1;
    cellGrid[strt_r+1][strt_c+0] = 1;
    cellGrid[strt_r+2][strt_c+1] = 1;
}

int main( int argc, char** argv ) {
    char** cellGrid;
    SDL_Window* window; SDL_Renderer* renderer;
    SDL_Rect* cells;
    SDL_Event e;
    bool quit;
    GAME_STATE state;

    cellGrid = initCellGrid();
    if( INIT_GRAPHICS(&window, &renderer, &cells) ) {
        free(cellGrid);
        return 1;
    }

    createToad(cellGrid, 5, 5);
    createBeacon(cellGrid, 10, 10);
    createBlock(cellGrid, 10, 5);
    createGlider(cellGrid, 0, 0);

    quit = false;
    state = pause;
    while( !quit ) {
        if( state == play ) {
            RENDER_CELLS(cellGrid, renderer, cells);
            /*         printCellGrid(cellGrid); */
            runSimulation(cellGrid);
            SDL_Delay(1000);
        }
        while ( SDL_PollEvent(&e) ) {
            if( e.type == SDL_QUIT )
                quit = true;
            else if( e.type == SDL_KEYDOWN ) {
                if( e.key.keysym.sym == SDLK_p )
                    state = (state == pause ? play : pause);
            }
        }
    }

    QUIT_GRAPHICS(window, renderer, cells);
    free(cellGrid);
    return 0;
}
