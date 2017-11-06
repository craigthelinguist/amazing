

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "graph.h"
#include "Keymap.h"

typedef struct GameState *GameState;

/// Make the `game_state` update itself by a single frame.
///
/// \param game_state: the current state of the game.
/// \param key_state: a snapshot of the keyboard in this frame.
void update_game(GameState game_state, KeyStateMap key_state);

GameState make_game_state(void);

void free_game_state(GameState game_state);

void init_maze(GameState game_state, int16_t maze_size, MazeAlgo maze_algo);

#endif
