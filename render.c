
#include <stdio.h>

#include "camera0.h"
#include "drawing.h"
#include "entity.h"
#include "game_state.h"
#include "graphical_constants.h"
#include "render.h"
#include "sprite0.h"
#include "stdbool.h"
#include "tile_map0.h"
#include <unistd.h>

// For the walls to look good, their width should be an even number.
const int64_t WALL_WIDTH = 1;

void draw_maze_layer(GUI gui, Camera camera, GameState game_state, struct image_sheet layer);
void draw_entities(GUI gui, GameState game_state);
void debug_shade_walkable(GUI gui, GameState game_state);
void debug_draw_grid(GUI gui, GameState game_state);

void render_game(GUI gui, GameState game_state) {
    clear_screen(gui);
    Camera camera = game_state->camera;
    draw_maze_layer(gui, camera, game_state, game_state->map_data->tileset.lower);
    draw_entities(gui, game_state);
    draw_maze_layer(gui, camera, game_state, game_state->map_data->tileset.upper);
    if (DRAW_WALKABLE) debug_shade_walkable(gui, game_state)
    if (DRAW_TILE_GRID) debug_draw_grid(gui, game_state)
    refresh_screen(gui);
}

void draw_maze_layer(GUI gui, Camera camera, GameState game_state, struct image_sheet layer) {

    const int MAZE_WD_PREFABS = game_state->map_data->maze_width_in_prefabs;
    tileset_index *tile_map = get_tile_map(game_state->map_data);

    for (int row = 0; row < MAZE_WD_PREFABS; row++) {
        for (int col = 0; col < MAZE_WD_PREFABS; col++) {
            tileset_index ti = tile_map[row * MAZE_WD_PREFABS + col];
            SDL_Rect draw_boundary = extract_img(&layer, ti);
            draw_image_offset(
                    gui,
                    camera,
                    &layer.img,
                    col * layer.img_size,
                    row * layer.img_size,
                    draw_boundary.x,
                    draw_boundary.y,
                    draw_boundary.w,
                    draw_boundary.h
            );
        }
    }
}

void draw_entities(GUI gui, GameState game_state) {

    sort_entities_by_depth(game_state);
    Camera camera = game_state->camera;

    for (int32_t index = 0; index < game_state->num_entities; index++) {
        Entity *entity = &game_state->entities[index];
        int image_top_y = entity->ypos;
        struct Sprite *sprite = entity->sprite;
        struct Offset offset = get_current_frame(sprite);

        draw_image_offset(gui, camera, sprite->sprite_sheet,
                          entity->xpos, entity->ypos,
                          offset.x, offset.y,
                          sprite->wd, sprite->ht);

        if (DRAW_COLLISION_BOXES) {
            SDL_Rect bbox = entity_bbox(entity);
            SDL_SetRenderDrawColor(gui->renderer, 0, 0, 255, 255);
            draw_rect(gui, camera, bbox.x, bbox.y, bbox.w, bbox.h);
        }

    }
}

void debug_shade_walkable(GUI gui, GameState game_state) {

    // Pull out the data we'll be using.
    Camera camera = game_state->camera;
    map_data *map_data = game_state->map_data;
    tileset_index *tilemap = get_tile_map(map_data);
    const int MAZE_WD_PREFABS = map_data->maze_width_in_prefabs;

    // DEBUG: draw walkable/non-walkable.
    SDL_SetRenderDrawColor(gui->renderer, 255, 0, 0, 100);
    const int TILES_PER_ROW = (PREFAB_WIDTH / MAP_TILE_SZ) * map_data->maze_width_in_prefabs;
    bool *wall_map = get_wall_map(map_data);

    for (int row = 0; row < TILES_PER_ROW; row++) {
        for (int col = 0; col < TILES_PER_ROW; col++) {

            // printf("row, col: %d, %d\n", row, col);
            int x = col * MAP_TILE_SZ;
            int y = row * MAP_TILE_SZ;
            // printf("x, y: %d, %d\n", x, y);
            struct pt_float pt = offset_point(gui, camera, x, y);
            SDL_Rect rect = (SDL_Rect) {(int)pt.x, (int)pt.y, MAP_TILE_SZ, MAP_TILE_SZ};

            if (wall_map[row * TILES_PER_ROW + col]) {
                SDL_SetRenderDrawColor(gui->renderer, 255, 0, 0, 25);
                SDL_RenderFillRect(get_renderer(gui), &rect);
            } else {
                SDL_SetRenderDrawColor(gui->renderer, 0, 255, 0, 25);
                SDL_RenderFillRect(get_renderer(gui), &rect);
            }

        }
    }
}

void debug_draw_grid(GUI gui, GameState game_state) {

    // Pull out the data we'll be using.
    Camera camera = game_state->camera;
    map_data *map_data = game_state->map_data;
    tileset_index *tilemap = get_tile_map(map_data);
    const int MAZE_WD_PREFABS = map_data->maze_width_in_prefabs;

    // DEBUG: draw walkable/non-walkable.
    SDL_SetRenderDrawColor(gui->renderer, 177, 120, 0, 255);
    const int TILES_PER_ROW = (PREFAB_WIDTH / MAP_TILE_SZ) * map_data->maze_width_in_prefabs;
    bool *wall_map = get_wall_map(map_data);

    for (int row = 0; row < TILES_PER_ROW; row++) {
        for (int col = 0; col < TILES_PER_ROW; col++) {

            // printf("row, col: %d, %d\n", row, col);
            int x = col * MAP_TILE_SZ;
            int y = row * MAP_TILE_SZ;
            // printf("x, y: %d, %d\n", x, y);
            struct pt_float pt = offset_point(gui, camera, x, y);
            SDL_Rect rect = (SDL_Rect) {(int)pt.x, (int)pt.y, MAP_TILE_SZ, MAP_TILE_SZ};
            SDL_RenderDrawRect(get_renderer(gui), &rect);

        }
    }
}
