
#ifndef RENDER_H
#define RENDER_H

#include "game_state0.h"
#include "gui.h"

// Debugging options.
#define DRAW_COLLISION_BOXES false
#define DRAW_WALKABLE false
#define DRAW_TILE_GRID false

void render_game(GUI gui, GameState game_state);

#endif

