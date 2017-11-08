
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "drawing.h"
#include "game_state0.h"
#include "imagelib.h"

// The index containing the player.
#define PLAYER_ENTITY_INDEX 0

static char *ASSET_DIRECTORY = "assets";

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

    if (key_state[SDL_SCANCODE_LEFT]) dx -= 1;
    if (key_state[SDL_SCANCODE_RIGHT]) dx += 1;
    if (key_state[SDL_SCANCODE_UP]) dy -= 1;
    if (key_state[SDL_SCANCODE_DOWN]) dy += 1;

    pan_camera(game_state->camera, dx, dy);
}

void add_entity(GameState game_state, GUI gui, int32_t xpos, int32_t ypos, char *image_name) {

    // Load the image.
    if (!imagelib_load(image_name, gui->renderer)) {
        switch (IMAGE_LIB_ERR) {
            case DIRECTORY_NOT_FOUND:
                fprintf(stderr, "Could not find directory %s", ASSET_DIRECTORY);
                break;
            case FILE_NOT_FOUND:
                fprintf(stderr, "Could not find asset `celes.png`");
                break;
            case FILE_NAME_TOO_LONG:
                fprintf(stderr, "`celes.png` is too long a file name.");
                break;
            case LIBRARY_FULL:
                fprintf(stderr, "The image library is full.");
                break;
            default:
                fprintf(stderr, "Unknown error %d", IMAGE_LIB_ERR);
                break;
        }
        exit(20);
    }

    // Create the entity.
    Image *image = imagelib_get(image_name);
    Entity player_entity = (Entity) {xpos, ypos, image};
    game_state->entities[game_state->num_entities++] = player_entity;

}

GameState make_game_state(GUI gui) {
    GameState game_state = calloc(1, sizeof(GameState));
    // add_entity(game_state, gui, 0, 0, "celes.png");
    return game_state;
}

void free_game_state(GameState game_state) {
    if (!game_state->graph) GRAPH_Free(game_state->graph);
    if (!game_state->camera) free_camera(game_state->camera);
    free(game_state);
}

/// Initialise the maze for the `game_state`. Memory will be allocated for the maze; it is freed when `game_state` is
/// freed.
void init_game(GameState game_state, int16_t maze_size, MazeAlgo maze_algo) {
    game_state->graph = GRAPH_Make(maze_size, maze_algo);
    game_state->camera = make_camera(0, 0);
    // TODO: centre camera on starting point.
}


