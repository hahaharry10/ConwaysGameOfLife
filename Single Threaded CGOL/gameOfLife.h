#ifndef CGOL_H
#define CGOL_H

void runSimulation(char** cellGrid); /* Run the simulation. */
char** initCellGrid(void); /* Allocate and initialise the cellGrid. */
void freeCellGrid(char** cell); /* Free allocated memory used for the cell grid. */
void printCellGrid(char** cellGrid); /* Output the cell grid to the terminal. */

#endif
