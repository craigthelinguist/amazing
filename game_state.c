
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "drawing.h"
#include "game_state0.h"
#include "imagelib.h"
#include "render.h"

// The index containing the player.
#define PLAYER_ENTITY_INDEX 0

static char *ASSET_DIRECTORY = "assets";

Image *get_image(char *image_name) {
    Image *image = imagelib_get(image_name);
    if (!image) {
        switch (IMAGE_LIB_ERR) {
            case NO_SUCH_IMAGE:
                fprintf(stderr, "Could not find the image `%s` (did you load it?)", image_name);
                exit(20);
            default:
                fprintf(stderr, "Unknown error while loading image `%s`", image_name);
                exit(20);
        }
    }
    return image;
}

void load_image(GUI gui, char *image_name) {
    if (!imagelib_load(image_name, gui->renderer)) {
        switch (IMAGE_LIB_ERR) {
            case DIRECTORY_NOT_FOUND:
                fprintf(stderr, "Could not find directory %s", ASSET_DIRECTORY);
                break;
            case FILE_NOT_FOUND:
                fprintf(stderr, "Could not find asset `%s`", image_name);
                break;
            case FILE_NAME_TOO_LONG:
                fprintf(stderr, "`%s` is too long a file name.", image_name);
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
}


void update_game(GameState game_state, KeyStateMap key_state) {

    // Check direction player should move in.
    int32_t dx = 0;
    int32_t dy = 0;
    if (key_state[SDL_SCANCODE_LEFT]) dx -= 1;
    if (key_state[SDL_SCANCODE_RIGHT]) dx += 1;
    if (key_state[SDL_SCANCODE_UP]) dy -= 1;
    if (key_state[SDL_SCANCODE_DOWN]) dy += 1;

    // Move player and camera.
    game_state->entities[PLAYER_ENTITY_INDEX].xpos += dx;
    game_state->entities[PLAYER_ENTITY_INDEX].ypos += dy;
    pan_camera(game_state->camera, dx, dy);

    // Update player image.

    Image *image;
    switch (LAST_DIRECTION_PRESSED) {
        case SDL_SCANCODE_LEFT:
            image = get_image("celes-left.png");
            break;
        case SDL_SCANCODE_RIGHT:
            image = get_image("celes-right.png");
            break;
        case SDL_SCANCODE_UP:
            image = get_image("celes-up.png");
            break;
        case SDL_SCANCODE_DOWN:
            image = get_image("celes-down.png");
            break;
        default:
            image = NULL;
            break;
    }

    if (image) {
        game_state->entities[PLAYER_ENTITY_INDEX].image = image;
    }

}

void add_entity(GameState game_state, GUI gui, int32_t xpos, int32_t ypos, char *image_name) {

    // Create the entity.
    Image *image = imagelib_get(image_name);
    Entity player_entity = (Entity) {xpos, ypos, image};
    game_state->entities[game_state->num_entities++] = player_entity;

}

GameState make_game_state(GUI gui) {
    GameState game_state = calloc(1, sizeof(struct GameState));
    game_state->num_entities = 0;
    return game_state;
}

void free_game_state(GameState game_state) {
    if (!game_state->graph) GRAPH_Free(game_state->graph);
    if (!game_state->camera) free_camera(game_state->camera);
    free(game_state);
}

/// Initialise the maze for the `game_state`. Memory will be allocated for the maze; it is freed when `game_state` is
/// freed.
void init_game(GameState game_state, GUI gui, int16_t maze_size, MazeAlgo maze_algo) {


    game_state->graph = GRAPH_Make(maze_size, maze_algo);
    game_state->camera = make_camera(0, 0);

    // This is the starting position of the player.
    POINT startPos = GRAPH_StartPos(game_state->graph);
    int start_pos_x = TILE_WIDTH/2 - COLLISION_SIZE/2;
    int start_pos_y = TILE_HEIGHT/2 - COLLISION_SIZE/2;

    // Load player images.
    load_image(gui, "celes-left.png");
    load_image(gui, "celes-right.png");
    load_image(gui, "celes-up.png");
    load_image(gui, "celes-down.png");

    // Create the player entity.
    add_entity(game_state, gui, start_pos_x, start_pos_y, "celes-down.png");
}


