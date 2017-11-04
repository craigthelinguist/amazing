

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "graph.h"

typedef struct GameState *GameState;

void update_game(GameState game_state);

GameState make_game_state(void);

void free_game_state(GameState game_state);

void init_maze(GameState game_state, int16_t maze_size, MazeAlgo maze_algo);

#endif

