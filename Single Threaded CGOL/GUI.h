#ifndef _GUI_H
#define _GUI_H

#include <stdio.h>
#include "SDL2/SDL.h"

#define SCREEN_WIDTH 1940
#define SCREEN_HEIGHT 1080
#define GRID_START_X 0
#define GRID_START_Y 40

#define CELL_WIDTH 10
#define CELL_HEIGHT 10
#define CELL_GRID_WIDTH ((SCREEN_WIDTH - GRID_START_X) / CELL_WIDTH)
#define CELL_GRID_HEIGHT ((SCREEN_HEIGHT - GRID_START_Y) / CELL_HEIGHT)

typedef enum { false, true } bool;
typedef enum { pause, play } GAME_STATE;

int INIT_GRAPHICS( SDL_Window** window, SDL_Renderer** renderer, SDL_Rect** cells );
void RENDER_CELLS( char** cellGrid, SDL_Renderer* renderer, SDL_Rect* cells );
void QUIT_GRAPHICS( SDL_Window* window, SDL_Renderer* renderer, SDL_Rect* cells );

#endif
