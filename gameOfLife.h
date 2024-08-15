#ifndef CGOL_H
#define CGOL_H

#define CELL_GRID_WIDTH 20
#define CELL_GRID_HEIGHT 20

void runSimulation(char** cellGrid); /* Run the simulation. */
char** initCellGrid(void); /* Allocate and initialise the cellGrid. */
void freeCellGrid(char** cell); /* Free allocated memory used for the cell grid. */
void printCellGrid(char** cellGrid); /* Output the cell grid to the terminal. */

#endif
