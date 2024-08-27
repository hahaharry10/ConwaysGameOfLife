#ifndef _GUI_H
#define _GUI_H

#include <stdio.h>
#include "SDL.h"

#define SCREEN_WIDTH    1940
#define SCREEN_HEIGHT   1080

#define CELL_GRID_WIDTH 20
#define CELL_GRID_HEIGHT 20
#define CELL_WIDTH 20
#define CELL_HEIGHT 20

typedef enum { false, true } bool;
typedef enum { pause, play } GAME_STATE;

int INIT_GRAPHICS( SDL_Window** window, SDL_Renderer** renderer, SDL_Rect** cells );
void RENDER_CELLS( char** cellGrid, SDL_Renderer* renderer, SDL_Rect* cells );
void QUIT_GRAPHICS( SDL_Window* window, SDL_Renderer* renderer, SDL_Rect* cells );

#endif
