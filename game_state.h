

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "graph.h"
#include "gui0.h"
#include "keymap.h"

typedef struct game_state *GameState;

/// Make the `game_state` update itself by a single frame.
///
/// \param game_state: the current state of the game.
/// \param key_state: a snapshot of the keyboard in this frame.
/// \param update_time: the time when the update began.
/// \param dt: time elapsed since the last call to `update_game`.
void update_game(GameState game_state, KeyStateMap key_state, long long update_time, long long dt);

GameState make_game_state(GUI gui);

void free_game_state(GameState game_state);

void init_game(GameState game_state, GUI gui, int16_t maze_size, MazeAlgo maze_algo);

#endif

