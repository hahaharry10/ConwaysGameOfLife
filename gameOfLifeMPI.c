#include "gameOfLifeMPI.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

void updateRow( char* cellRow, char* above, char* below) {
    int i, x;
    char* neighbourGrid;

    for( i = 0; i < CELL_GRID_HEIGHT*CELL_GRID_WIDTH; i++ )
        neighbourGrid[i] = 0;

    /* Get neighbour count for all cells: */
    for( i = 0; i < CELL_GRID_WIDTH; i++ ) {
        x = above[i] + below[i];
        neighbourGrid[i] += x;
        x += cellRow[i];
        if( i != 0 ) { neighbourGrid[i-1] += x; }
        if( i != CELL_GRID_WIDTH ) { neighbourGrid[i-1] += x; }
    }

    for( i = 0; i < CELL_GRID_WIDTH; i++ ) {
        if( cellRow[i] && (neighbourGrid[i] < 2 || neighbourGrid[i] > 3) ) { cellRow[i] = 0; }
        if( !cellRow[i] && neighbourGrid[i] == 3 ) { cellRow[i] = 1; }
    }
}

char* initCellGrid(void) {
    int i;
    char* cellGrid; /* Char used to allocate 8 bits per cell */

    cellGrid = (char *) malloc(CELL_GRID_HEIGHT*CELL_GRID_WIDTH);
    for( i = 0; i < CELL_GRID_HEIGHT*CELL_GRID_WIDTH; i++ )
        *(cellGrid+i) = 0;

    return cellGrid;
}

void freeCellGrid(char* cellGrid) {
    free(cellGrid);
}

void printCellGrid(char* cellGrid) {
    int i, j;

    for( i = 0; i < CELL_GRID_HEIGHT; i++ ) {
        for( j = 0; j < CELL_GRID_WIDTH; j++ )
            printf("%i ", *(cellGrid+(i*CELL_GRID_WIDTH)+j));
        printf("\n");
    }
}

void createToad( char* cellGrid, int strt_r, int strt_c ) {
    *(cellGrid+((strt_r+1)*CELL_GRID_WIDTH)+strt_c+0) = 1;
    *(cellGrid+((strt_r+1)*CELL_GRID_WIDTH)+strt_c+1) = 1;
    *(cellGrid+((strt_r+1)*CELL_GRID_WIDTH)+strt_c+2) = 1;
    *(cellGrid+((strt_r+0)*CELL_GRID_WIDTH)+strt_c+1) = 1;
    *(cellGrid+((strt_r+0)*CELL_GRID_WIDTH)+strt_c+2) = 1;
    *(cellGrid+((strt_r+0)*CELL_GRID_WIDTH)+strt_c+3) = 1;
}

void createBeacon( char* cellGrid, int strt_r, int strt_c ) {
    *(cellGrid+((strt_r+0)*CELL_GRID_WIDTH)+strt_c+0) = 1;
    *(cellGrid+((strt_r+1)*CELL_GRID_WIDTH)+strt_c+0) = 1;
    *(cellGrid+((strt_r+0)*CELL_GRID_WIDTH)+strt_c+1) = 1;
    *(cellGrid+((strt_r+3)*CELL_GRID_WIDTH)+strt_c+2) = 1;
    *(cellGrid+((strt_r+3)*CELL_GRID_WIDTH)+strt_c+3) = 1;
    *(cellGrid+((strt_r+2)*CELL_GRID_WIDTH)+strt_c+3) = 1;
}

void createBlock( char* cellGrid, int strt_r, int strt_c ) {
    *(cellGrid+((strt_r+0)*CELL_GRID_WIDTH)+strt_c+0) = 1;
    *(cellGrid+((strt_r+1)*CELL_GRID_WIDTH)+strt_c+1) = 1;
    *(cellGrid+((strt_r+1)*CELL_GRID_WIDTH)+strt_c+0) = 1;
    *(cellGrid+((strt_r+1)*CELL_GRID_WIDTH)+strt_c+1) = 1;
}

void createGlider( char* cellGrid, int strt_r, int strt_c ) {
    *(cellGrid+((strt_r+0)*CELL_GRID_WIDTH)+strt_c+2) = 1;
    *(cellGrid+((strt_r+1)*CELL_GRID_WIDTH)+strt_c+2) = 1;
    *(cellGrid+((strt_r+2)*CELL_GRID_WIDTH)+strt_c+2) = 1;
    *(cellGrid+((strt_r+1)*CELL_GRID_WIDTH)+strt_c+0) = 1;
    *(cellGrid+((strt_r+2)*CELL_GRID_WIDTH)+strt_c+1) = 1;
}

int main( int argc, char** argv ) {
    char* cellGrid;
    char* cellRow;
    int i;

    /* TODO: In order to parallelise:
     *  1) Flatten array to a one dimension array.
     *  2) Send each rank their cells and their required ghost cells.
     *  3) Update row.
     *  4) Send updated row to neighbouring ranks to update their ghost cells.
     */

    // Initialise MPI and get the rank and no. of processes.
    int rank, numProcs;
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &numProcs );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank     );

    cellRow = (char *) malloc(CELL_GRID_WIDTH);

    if( rank == 0 ) {
        cellGrid = initCellGrid();
        createToad(cellGrid, 5, 5);
        createBeacon(cellGrid, 10, 10);
        createBlock(cellGrid, 10, 5);
        createGlider(cellGrid, 0, 0);
        printCellGrid(cellGrid);
    }

    MPI_Scatter(cellGrid, (int) CELL_GRID_HEIGHT / numProcs, MPI_CHAR, cellRow, CELL_GRID_WIDTH / numProcs, MPI_CHAR, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    printf("Rank %i Given row:\n\t%s", rank, cellRow);
    free(cellRow);
    freeCellGrid(cellGrid);
	MPI_Finalize();
    return 0;

    while( 1 ) {
        printCellGrid(cellGrid);
        runSimulation(cellGrid);
/*         break; */
        sleep(2);
    }

    return 0;
}
