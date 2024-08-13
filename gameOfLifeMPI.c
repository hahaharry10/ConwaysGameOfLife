#include "gameOfLifeMPI.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* For sleep() function. */
#include <mpi.h>

void updateBoundaryDomain( char* domain, int domainSize, int rank ) {
    int i, row, col;
    char* neighbourGrid;

    neighbourGrid = malloc(domainSize-CELL_GRID_WIDTH);
    for( i = 0; i < domainSize-(2*CELL_GRID_WIDTH); i++ )
        neighbourGrid[i] = 0;

    /* Get neighbour count for all cells: */
    for( i = 0; i < domainSize; i++ ) {
        row = i / CELL_GRID_WIDTH;
        col = i % CELL_GRID_WIDTH;
        if( domain[i] ) {
            if( row != 0 ) {
                ++neighbourGrid[i-CELL_GRID_WIDTH];
                if( col != 0 ) { ++neighbourGrid[i-CELL_GRID_WIDTH-1]; }
                if( col != CELL_GRID_WIDTH-1 ) { ++neighbourGrid[i-CELL_GRID_WIDTH+1]; }
            }
            if( row != (domainSize/CELL_GRID_WIDTH)-1 ) {
                ++neighbourGrid[i-CELL_GRID_WIDTH];
                if( col != 0 ) { ++neighbourGrid[i+CELL_GRID_WIDTH-1]; }
                if( col != CELL_GRID_WIDTH-1 ) { ++neighbourGrid[i+CELL_GRID_WIDTH+1]; }
            }
            if( col != 0 ) { ++neighbourGrid[i-1]; }
            if( col != CELL_GRID_WIDTH-1 ) { ++neighbourGrid[i+1]; }
        }
    }

    /* for( i = 0; i < domainSize; i++ ) {
        printf("%i ", (int) neighbourGrid[i]);
        if( (i+1)%CELL_GRID_WIDTH == 0 )
            printf("\n");
    } */

    for( i = 0; i < domainSize; i++ ) {
        if( domain[i] && (neighbourGrid[i] < 2 || neighbourGrid[i] > 3) ) { domain[i] = 0; }
        if( !domain[i] && neighbourGrid[i] == 3 ) { domain[i] = 1; }
    }
}

void updateCentreDomain( char* domain, int domainSize ) {
    int i, x, row, col;
    char* neighbourGrid;

    neighbourGrid = malloc(domainSize);
    for( i = 0; i < domainSize; i++ )
        neighbourGrid[i] = 0;

    /* Get neighbour count for all cells: */
    for( i = 0; i < domainSize; i++ ) {
        row = i / CELL_GRID_WIDTH;
        col = i % CELL_GRID_WIDTH;
        if( domain[i] ) {
            if( row != 0 ) {
                ++neighbourGrid[i-CELL_GRID_WIDTH];
                if( col != 0 ) { ++neighbourGrid[i-CELL_GRID_WIDTH-1]; }
                if( col != CELL_GRID_WIDTH-1 ) { ++neighbourGrid[i-CELL_GRID_WIDTH+1]; }
            }
            if( row != (domainSize/CELL_GRID_WIDTH)-1 ) {
                ++neighbourGrid[i+CELL_GRID_WIDTH];
                if( col != 0 ) { ++neighbourGrid[i+CELL_GRID_WIDTH-1]; }
                if( col != CELL_GRID_WIDTH-1 ) { ++neighbourGrid[i+CELL_GRID_WIDTH+1]; }
            }
            if( col != 0 ) { ++neighbourGrid[i-1]; }
            if( col != CELL_GRID_WIDTH-1 ) { ++neighbourGrid[i+1]; }
        }
    }

    /* for( i = 0; i < domainSize; i++ ) {
        printf("%i ", (int) neighbourGrid[i]);
        if( (i+1)%CELL_GRID_WIDTH == 0 )
            printf("\n");
    } */

    for( i = CELL_GRID_WIDTH; i < domainSize-CELL_GRID_WIDTH; i++ ) {
        if( domain[i] && (neighbourGrid[i] < 2 || neighbourGrid[i] > 3) ) { domain[i] = 0; }
        if( !domain[i] && neighbourGrid[i] == 3 ) { domain[i] = 1; }
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
    char* domain;
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

    if( rank == 0 ) {
        MPI_Request* requests;
        int *domainSizes, *displs;

        cellGrid = initCellGrid();
        createToad(cellGrid, 5, 5);
        createBeacon(cellGrid, 10, 10);
        createBlock(cellGrid, 10, 5);
        createGlider(cellGrid, 0, 0);
        createLightweightSpaceship(cellGrid, 14, 15);
        printCellGrid(cellGrid);

        domainSize = (CELL_GRID_HEIGHT / numProcs);
        domainSizes = (int *) calloc(numProcs, sizeof(int));
        /* Split rows evenly accross all processes and add ghost cells: */
        for( i = 0; i < numProcs; i++ )
            domainSizes[i] = (domainSize * CELL_GRID_WIDTH) + ( (i == 0 || i == numProcs-1) ? CELL_GRID_WIDTH : 2*CELL_GRID_WIDTH ) ;
        /* add remainding rows to other processes: */
        for( i = 0; i < CELL_GRID_HEIGHT-(domainSize*numProcs); i++ )
            domainSizes[i%numProcs] += CELL_GRID_WIDTH;

        /* Asynchonously send load size to all other ranks */
        requests = (MPI_Request *) calloc(numProcs, sizeof(MPI_Request));
        for( i = 1; i < numProcs; i++ )
            MPI_Isend(domainSizes+i, 1, MPI_INT, i, 0, MPI_COMM_WORLD, requests+i-1);

        domainSize = domainSizes[0];

        displs = (int *) calloc(numProcs, sizeof(int));
        displs[0] = 0;
        printf("Rank 0 gets (in rows):\n\tCounts: %i\n\tDispls: %i\n", domainSizes[0]/CELL_GRID_WIDTH, displs[0]/CELL_GRID_WIDTH);
        for( i = 1; i < numProcs; i++ ) {
            displs[i] = (-2 * i * CELL_GRID_WIDTH);
            for( j = 0; j < i; j++ )
                displs[i] += domainSizes[j];
            printf("Rank %i gets (in rows):\n\tCounts: %i\n\tDispls: %i\n", i, domainSizes[i]/CELL_GRID_WIDTH, displs[i]/CELL_GRID_WIDTH);
        }
        domain = (char *) malloc(domainSize);

        MPI_Waitall(numProcs-1, requests, MPI_STATUSES_IGNORE);
        MPI_Scatterv(cellGrid, domainSizes, displs, MPI_CHAR, domain, domainSize, MPI_CHAR, 0, MPI_COMM_WORLD);
        free(domainSizes);
        free(requests);
    }
    else {
        MPI_Recv(&domainSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        domain = (char *) malloc(domainSize);
        MPI_Scatterv(NULL, NULL, NULL, MPI_CHAR, domain, domainSize, MPI_CHAR, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    for( i = 0; i < numProcs; i++ ) {
        if( i == rank ) {
            printf("Rank %i: Domain Size %i\n", rank, domainSize);
            for( j = 0; j < domainSize; j++ ) {
                if( j % CELL_GRID_WIDTH == 0 )
                    printf("\n");
                printf("%i ", domain[j]);
            }
            printf("\nSimulated 1 Generation:\n");
            if( rank == 0 || rank == numProcs-1 ) { updateBoundaryDomain(domain, domainSize, rank); }
            else { updateCentreDomain(domain, domainSize); }
            for( j = 0; j < domainSize; j++ ) {
                if( j % CELL_GRID_WIDTH == 0 )
                    printf("\n");
                printf("%i ", domain[j]);
            }
            printf("\n\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    /* TODO: Binary tree reduction to rank 0: */

    free(domain);
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
