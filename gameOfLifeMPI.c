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

void createLightweightSpaceship( char* cellGrid, int strt_r, int strt_c ) {
    *(cellGrid+((strt_r+0)*CELL_GRID_WIDTH)+strt_c+0) = 1;
    *(cellGrid+((strt_r+0)*CELL_GRID_WIDTH)+strt_c+1) = 1;
    *(cellGrid+((strt_r+0)*CELL_GRID_WIDTH)+strt_c+2) = 1;
    *(cellGrid+((strt_r+0)*CELL_GRID_WIDTH)+strt_c+3) = 1;
    *(cellGrid+((strt_r+1)*CELL_GRID_WIDTH)+strt_c+0) = 1;
    *(cellGrid+((strt_r+1)*CELL_GRID_WIDTH)+strt_c+4) = 1;
    *(cellGrid+((strt_r+2)*CELL_GRID_WIDTH)+strt_c+0) = 1;
    *(cellGrid+((strt_r+3)*CELL_GRID_WIDTH)+strt_c+1) = 1;
    *(cellGrid+((strt_r+3)*CELL_GRID_WIDTH)+strt_c+4) = 1;
}

int main( int argc, char** argv ) {
    char* cellGrid;
    char* cellRow;
    int i, j, domainSize;

    /* TODO: In order to parallelise:
     *  - [ ] Send each rank their cells and their required ghost cells.
     *  - [ ] Send updated row to neighbouring ranks to update their ghost cells.
     */

    /* Initialise MPI and get the rank and no. of processes. */
    int rank, numProcs;
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &numProcs );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank     );

    MPI_Request* requests;
    int* domainSizes;
    if( rank == 0 ) {
        cellGrid = initCellGrid();
        createToad(cellGrid, 5, 5);
        createBeacon(cellGrid, 10, 10);
        createBlock(cellGrid, 10, 5);
        createGlider(cellGrid, 0, 0);
        createLightweightSpaceship(cellGrid, 14, 15);
        printCellGrid(cellGrid);

        domainSize = (CELL_GRID_HEIGHT / numProcs);
        domainSizes = (int *) calloc(numProcs, sizeof(int));
        for( i = 0; i < numProcs; i++ )
            domainSizes[i] = domainSize * CELL_GRID_WIDTH; /* Set default domain size */
        for( i = 0; i < CELL_GRID_HEIGHT-(domainSize*numProcs); i++ )
            domainSizes[i%numProcs] += CELL_GRID_WIDTH; /* Balance excess load accross all processes */

        /* Asynchonously send load size to all other ranks */
        requests = (MPI_Request *) calloc(numProcs, sizeof(MPI_Request));
        for( i = 1; i < numProcs; i++ )
            MPI_Isend(domainSizes+i, 1, MPI_INT, i, 0, MPI_COMM_WORLD, requests+i-1);

        domainSize = domainSizes[0];
        MPI_Waitall(numProcs-1, requests, MPI_STATUSES_IGNORE);

        /*
         * The following variables are being reused for MPI_Scatterv:
         *      requests -> displs
         *      domainSizes -> counts
         */
        int* displs = (int *) requests;
        for( i = 0; i < numProcs; i++ ) {
            displs[i] = 0; 
            for( j = 0; j < i; j++ )
                displs[i] += domainSizes[j];
            printf("Rank %i gets:\n\tCounts: %i\n\tDispls: %i\n", i, domainSizes[i], displs[i]);
        }
        cellRow = (char *) malloc(domainSize);

        MPI_Scatterv(cellGrid, domainSizes, displs, MPI_CHAR, cellRow, domainSize, MPI_CHAR, 0, MPI_COMM_WORLD);
        free(domainSizes);
        free(requests);
    }
    else {
        MPI_Recv(&domainSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cellRow = (char *) malloc(domainSize);
        MPI_Scatterv(NULL, NULL, NULL, MPI_CHAR, cellRow, domainSize, MPI_CHAR, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    for( i = 0; i < numProcs; i++ ) {
        if( i == rank ) {
            printf("Rank %i: Domain Size %i\n", rank, domainSize);
            for( j = 0; j < domainSize; j++ ) {
                if( j % CELL_GRID_WIDTH == 0 )
                    printf("\n");
                printf("%i ", cellRow[j]);
            }
            printf("\n\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    /* TODO: Binary tree reduction to rank 0: */

    free(cellRow);
    if( rank == 0 )
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
