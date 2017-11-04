
#include <stdlib.h>

#include "game_state0.h"

void update_game(GameState game_state) {
  
}

GameState make_game_state() {
  GameState game_state = malloc(sizeof(GameState));
  return game_state;
}

void free_game_state(GameState game_state) {
  GRAPH_Free(game_state->graph);
  free(game_state);
}

/// Initialise the maze for the `game_state`. Memory will be allocated for the maze.
/// It is freed when `game_state` is freed.
void init_maze(GameState game_state, int16_t maze_size, MazeAlgo maze_algo) {
  game_state->graph = GRAPH_Make(maze_size, maze_algo);
}


