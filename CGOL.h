#ifndef CGOL_H
#define CGOL_H

#define CELL_GRID_WIDTH 100
#define CELL_GRID_HEIGHT 100

void runSimulation(int** cellGrid); /* Run the simulation. */
int** initCellGrid(); /* Allocate and initialise the cellGrid. */
void freeCellGrid(int** cell); /* Free allocated memory used for the cell grid. */
void printCellGrid(int** cellGrid); /* Output the cell grid to the terminal. */

#endif
