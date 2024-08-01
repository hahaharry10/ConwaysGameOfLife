#include "CGOL.h"
#include <stdio.h>
#include <stdlib.h>

void runSimulation(int** cellGrid) {
}

int** initCellGrid() {
    int i;
    int** cellGrid;

    cellGrid = (int **) calloc(CELL_GRID_HEIGHT, sizeof(int *));
    for( i = 0; i < CELL_GRID_HEIGHT; i++ ) 
        cellGrid[i] = (int *) calloc(CELL_GRID_WIDTH, sizeof(int));

    return cellGrid;
}

void freeCellGrid(int** cellGrid) {
    int i;

    for( i = 0; i < CELL_GRID_HEIGHT; i++ ) 
        free(cellGrid[i]);
    free(cellGrid);
}

void printCellGrid(int** cellGrid) {
    int i, j;

    for( i = 0; i < CELL_GRID_HEIGHT; i++ ) {
        for( j = 0; j < CELL_GRID_WIDTH; j++ )
            printf("%i ", cellGrid[i][j]);
        printf("\n");
    }
}

int main( int argc, char** argv ) {
    char* hello;

    hello = argv[1];
    return 0;
}
