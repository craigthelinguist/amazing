
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "game_state0.h"

void update_game(GameState game_state, KeyStateMap key_state) {
    int32_t dx = 0;
    int32_t dy = 0;

    if (game_state == NULL) {
        fprintf(stderr, "Updating game state, but game_state is null");
        exit(10);
    }
    if (game_state->camera == NULL) {
        fprintf(stderr, "Updating game state, but the camera is null");
        exit(10);
    }

    // TODO: `key_state` needs SDL_Scancode, which is a physical key rather than a virtual key.
    // TODO: does this mean we need to store our own virtual key_state?
    if (key_state[SDL_SCANCODE_LEFT]) dx -= 1;
    if (key_state[SDL_SCANCODE_RIGHT]) dx += 1;
    if (key_state[SDL_SCANCODE_UP]) dy -= 1;
    if (key_state[SDL_SCANCODE_DOWN]) dy += 1;

    pan_camera(game_state->camera, dx, dy);
}

GameState make_game_state() {
    GameState game_state = calloc(1, sizeof(GameState));
    return game_state;
}

void free_game_state(GameState game_state) {
    if (!game_state->graph) GRAPH_Free(game_state->graph);
    if (!game_state->camera) free_camera(game_state->camera);
    free(game_state);
}

/// Initialise the maze for the `game_state`. Memory will be allocated for the maze; it is freed when `game_state` is
/// freed.
void init_maze(GameState game_state, int16_t maze_size, MazeAlgo maze_algo) {
    game_state->graph = GRAPH_Make(maze_size, maze_algo);
    game_state->camera = make_camera(0, 0);
    // TODO: centre camera on starting point.
}


