
#ifndef RENDER_H
#define RENDER_H

#include "game_state0.h"
#include "gui.h"

// Drawing parameters for tiles in the maze.
#define TILE_WIDTH 64
#define TILE_HEIGHT 64

// Whether you should draw collision boxes on entities.
#define DRAW_COLLISION_BOXES true

#define COLLISION_SIZE 32

void render_game(GUI gui, GameState game_state);

#endif

