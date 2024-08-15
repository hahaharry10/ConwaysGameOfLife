#ifndef CGOL_H
#define CGOL_H

#define CELL_GRID_WIDTH 20
#define CELL_GRID_HEIGHT 20

void updateBoundaryDomain( char* domain, int domainSize, int rank ); /* Update domain for boundary ranks (ranks that contain the first few or last few cells. */
void updateCentreDomain( char* domain, int domainSize ); /* Update domain for non-boundary ranks. */
char* initCellGrid(void); /* Allocate and initialise the cellGrid. */
void freeCellGrid(char* cellGrid); /* Free allocated memory used for the cell grid. */
void printCellGrid(char* cellGrid); /* Output the cell grid to the terminal. */

void createToad(char* cellGrid, int start_r, int start_c);
void createBeacon(char* cellGrid, int start_r, int start_c);
void createBlock(char* cellGrid, int start_r, int start_c);
void createGlider(char* cellGrid, int start_r, int start_c);
void createLightweightSpaceship(char* cellGrid, int start_r, int start_c);

#endif
