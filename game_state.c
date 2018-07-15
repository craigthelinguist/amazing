
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "drawing.h"
#include "game_state0.h"
#include "gui0.h"
#include "imagelib.h"
#include "render.h"
#include "sprite.h"
#include "tile_map0.h"

// The index containing the player.
#define PLAYER_ENTITY_INDEX 0

static char *ASSET_DIRECTORY = "assets";

image *get_image(char *image_name) {
    image *image = imagelib_get(image_name);
    if (!image) {
        switch (IMAGELIB_ERRCODE) {
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
        IMAGELIB_ERR("Failed loading %s\n", image_name);
    }
}


void update_game(GameState game_state, KeyStateMap key_state, long long update_time) {

    // Check direction player should move in.
    // TODO: if e.g. left and right are both pressed down, then the most recently pressed takes precedence.
    int32_t dx = 0;
    int32_t dy = 0;
    if (key_state[SDL_SCANCODE_LEFT]) dx -= 1;
    if (key_state[SDL_SCANCODE_RIGHT]) dx += 1;
    if (key_state[SDL_SCANCODE_UP]) dy -= 1;
    if (key_state[SDL_SCANCODE_DOWN]) dy += 1;

    // Check if player can move.
    SDL_Rect bbox = entity_bbox_after_move(&game_state->entities[PLAYER_ENTITY_INDEX], dx, dy);

    if (!is_box_colliding(game_state->map_data, bbox)) {
        pan_camera(game_state->camera, dx, dy);
        game_state->entities[PLAYER_ENTITY_INDEX].xpos += dx;
        game_state->entities[PLAYER_ENTITY_INDEX].ypos += dy;
    }

    
    // Update player sprite's animation name.
    Sprite sprite = game_state->entities[PLAYER_ENTITY_INDEX].sprite;

    // If LAST_DIRECTION_PRESSED is currently held down, that is where the sprite should face.
    if (KEY_DOWN_MAP[LAST_DIRECTION_PRESSED]) {
        switch (LAST_DIRECTION_PRESSED) {
            case SDL_SCANCODE_LEFT:
                set_anim_name(sprite, "left", update_time);
                break;
            case SDL_SCANCODE_RIGHT:
                set_anim_name(sprite, "right", update_time);
                break;
            case SDL_SCANCODE_UP:
                set_anim_name(sprite, "up", update_time);
                break;
            case SDL_SCANCODE_DOWN:
                set_anim_name(sprite, "down", update_time);
                break;
            default:
                break;
        }
    }

    // Otherwise if moving in either orthogonal direction , that is where the sprite should face.
    else {
        if (dx > 0) {
            set_anim_name(sprite, "right", update_time);
        } else if (dx < 0) {
            set_anim_name(sprite, "left", update_time);
        } else if (dy < 0) {
            set_anim_name(sprite, "up", update_time);
        } else if (dy > 0) {
            set_anim_name(sprite, "down", update_time);
        }
    }

    // If the player is standing still, reset their animation. Otherwise animate the sprite.
    if (dx == 0 && dy == 0) {
        reset_animation(sprite);
    } else {
        animate(sprite, update_time);
    }
    
}

void add_entity(GameState game_state, GUI gui, int32_t xpos, int32_t ypos,
                uint8_t collision_wd, uint8_t collision_ht, char *sprite_sheet_name) {
    Sprite sprite = load_sprite(sprite_sheet_name, gui->renderer);
    if (!sprite) {
        IMAGELIB_ERR("Failed to load sprite `%s`", sprite_sheet_name);
    }
    Entity player_entity = (Entity) {xpos, ypos, collision_wd, collision_ht, sprite};
    game_state->entities[game_state->num_entities++] = player_entity;
}

GameState make_game_state(GUI gui) {
    GameState game_state = calloc(1, sizeof(struct GameState));
    game_state->num_entities = 0;
    return game_state;
}

void free_game_state(GameState game_state) {
    if (!game_state->graph) graph_free(game_state->graph);
    if (!game_state->camera) free_camera(game_state->camera);
    free(game_state);
}

/// Initialise the maze for the `game_state`. Memory will be allocated for the maze; it is freed when `game_state` is
/// freed.
void init_game(GameState game_state, GUI gui, int16_t maze_size, MazeAlgo maze_algo) {

    game_state->graph = graph_make(maze_size, maze_algo);

    // The player's starting position is in the middle of the starting tile.
    // The camera should be centred on their starting position.
    {
        POINT start_pos = graph_start_pos(game_state->graph);
        int pos_x = start_pos.x * PREFAB_WIDTH + PREFAB_WIDTH / 2 - SPRITE_WD / 2;
        int pos_y = start_pos.y * PREFAB_WIDTH + PREFAB_WIDTH / 2 - SPRITE_HT / 2;
        game_state->camera = make_camera(pos_x, pos_y);
        add_entity(game_state, gui, pos_x, pos_y, SPRITE_COLLISION_WD, SPRITE_COLLISION_HT, "celes");
    }

    // Add the statue. Its position is in the middle of the exit tile.
    {
        POINT exit_pos = game_state->graph->exit_pos;
        int pos_x = exit_pos.x * PREFAB_WIDTH + PREFAB_WIDTH / 2 - STATUE_WD / 2;
        int pos_y = exit_pos.y * PREFAB_WIDTH + PREFAB_WIDTH / 2 - STATUE_HT / 2;
        add_entity(game_state, gui, pos_x, pos_y, STATUE_COLLISION_WD, STATUE_COLLISION_HT, "statue");
    }

    // Construct the tile_map and wall_map.
    const char *FNAME_PREFABS = "maze-grid";
    const char *FNAME_PATHING = "maze-grid-pathing.png";
    game_state->map_data = generate_map_data(game_state->graph, gui->renderer, FNAME_PREFABS, FNAME_PATHING);

    // Debugging.
    {
        POINT start_pos = game_state->graph->start_pos;
        POINT exit_pos = game_state->graph->exit_pos;
        fprintf(stdout, "\n");
        fprintf(stdout, "Start: (%d, %d)\n", start_pos.x, start_pos.y);
        fprintf(stdout, "Exit: (%d, %d)\n", exit_pos.x, exit_pos.y);
    }

}
