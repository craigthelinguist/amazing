
#include <stdio.h>

#include "camera0.h"
#include "drawing.h"
#include "entity.h"
#include "game_state.h"
#include "graphical_constants.h"
#include "render.h"
#include "sprite0.h"
#include "stdbool.h"
#include <unistd.h>

// Some default colours.
Colour COLOUR_FLOOR = {200, 200, 200, 255};
Colour COLOUR_WALL = {0, 0, 0, 255};
Colour COLOUR_FLOOR_SPECIAL = {240, 200, 200, 255};
Colour COLOUR_BACKGROUND = {0, 0, 0, 255};

// For the walls to look good, their width should be an even number.
const int64_t WALL_WIDTH = 1;

// Forward declarations.
void draw_tile_walls(GUI gui, GameState game_state, int64_t x, int64_t y);

void draw_maze(GUI gui, GameState game_state);

void draw_maze_old(GUI gui, GameState game_state);

void draw_entities(GUI gui, GameState game_state);

void render_game(GUI gui, GameState game_state) {
    clear_screen(gui);
    draw_maze(gui, game_state);
    draw_entities(gui, game_state);
    refresh_screen(gui);
}

#define MIN(X, Y) (X < Y ? X : Y)
#define MAX(X, Y) (X > Y ? X : Y)

void draw_maze(GUI gui, GameState game_state) {

    // Pull out the data we'll be using.
    Camera camera = game_state->camera;
    map_data *map_data = game_state->map_data;
    tileset_index *tilemap = get_tile_map(map_data);
    image_sheet tileset = map_data->tile_set;
    const int MAZE_WD_PREFABS = map_data->maze_width_in_prefabs;

    for (int row = 0; row < MAZE_WD_PREFABS; row++) {
        for (int col = 0; col < MAZE_WD_PREFABS; col++) {
            tileset_index ti = tilemap[row * MAZE_WD_PREFABS + col];
            SDL_Rect draw_boundary = extract_img(&tileset, ti);

            draw_image_offset(
                    gui,
                    camera,
                    &tileset.img,
                    col * tileset.img_size,
                    row * tileset.img_size,
                    draw_boundary.x,
                    draw_boundary.y,
                    draw_boundary.w,
                    draw_boundary.h
            );
        }
    }

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

            Point_Int32 pt = offset_point(gui, camera, x, y);
            SDL_Rect rect = (SDL_Rect) {pt.x, pt.y, MAP_TILE_SZ, MAP_TILE_SZ};

            if (wall_map[row * TILES_PER_ROW + col]) {
                SDL_SetRenderDrawColor(gui->renderer, 255, 0, 0, 25);
                SDL_RenderFillRect(get_renderer(gui), &rect);
            } else {
                SDL_SetRenderDrawColor(gui->renderer, 0, 255, 0, 25);
                SDL_RenderFillRect(get_renderer(gui), &rect);
            }

        }
    }

    // SDL_Rect rect = {pt.x, pt.y, wd, ht};
    // SDL_RenderDrawRect(get_renderer(gui), &rect);

}

void draw_entities(GUI gui, GameState game_state) {
    Camera camera = game_state->camera;
    for (int32_t index = 0; index < game_state->num_entities; index++) {
        Entity *entity = &game_state->entities[index];
        int IMAGE_WD = SPRITE_WD;
        int IMAGE_HT = SPRITE_HT;
        int image_top_y = entity->ypos;
        image *image = entity->sprite->sprite_sheet;

        // The current frame of the current animation.
        struct Offset offset = get_current_frame(entity->sprite);

        draw_image_offset(gui, camera, image,
                entity->xpos, image_top_y,
                offset.x, offset.y,
                SPRITE_WD, SPRITE_HT);

        // set_drawcol(gui, 0, 0, 255, 0);

        // DEBUG: draw entity box.
        SDL_Rect ebox = entity_pos(entity);
        SDL_SetRenderDrawColor(gui->renderer, 255, 255, 0, 255);
        draw_rect(gui, camera, ebox.x, ebox.y, ebox.w, ebox.h);

        // DEBUG: draw collision box.
        SDL_Rect bbox = entity_bbox(entity);
        SDL_SetRenderDrawColor(gui->renderer, 0, 0, 255, 255);
        draw_rect(gui, camera, bbox.x, bbox.y, bbox.w, bbox.h);

    }
}

void draw_maze_old(GUI gui, GameState game_state) {

    GRAPH graph = game_state->graph;
    Camera camera = game_state->camera;

    /* Debug info */
    //fprintf(stdout, "Drawing the graph, which has width %d\n", graph_width(graph));

    // Draw the background.
    const int8_t WIDTH = graph_width(graph);
    set_drawcol2(gui, COLOUR_FLOOR);
    fill_rect(gui, camera, 0, 0, WIDTH * TILE_WIDTH, WIDTH * TILE_HEIGHT);

    /* Debug info
    POINT start = graph_start_pos(graph);
    POINT exit = graph_exit_pos(graph);
    fprintf(stdout, "Start: (%d, %d) and End: (%d, %d)\n", start.x, start.y, exit.x, exit.y);
    */

    // Draw the start and end tiles.
    set_drawcol2(gui, COLOUR_FLOOR_SPECIAL);
    POINT start_tile = graph_start_pos(graph);
    POINT exit_tile = graph_exit_pos(graph);
    fill_rect(gui, camera,
              +TILE_WIDTH * start_tile.x, TILE_HEIGHT * start_tile.y, TILE_WIDTH, TILE_HEIGHT);
    fill_rect(gui, camera,
              +TILE_WIDTH * exit_tile.x, TILE_HEIGHT * exit_tile.y, TILE_WIDTH, TILE_HEIGHT);

}

void draw_tile_walls(GUI gui, GameState game_state, int64_t x, int64_t y) {

    GRAPH graph = game_state->graph;
    Camera camera = game_state->camera;

    TILE tile = graph_has_tile_at(graph, x, y);
    POINT p = make_point(x, y);

    // Draw top wall.
    if (!in_bounds(graph, point_after_moving(p, TOP)) || !is_connected(tile, TOP)) {
        fill_rect(gui, camera,
                  x * TILE_WIDTH,
                  y * TILE_HEIGHT - WALL_WIDTH / 2,
                  TILE_WIDTH,
                  WALL_WIDTH);
    }

    // Draw bottom wall.
    if (!in_bounds(graph, point_after_moving(p, DOWN)) || !is_connected(tile, DOWN)) {
        fill_rect(gui, camera,
                  x * TILE_WIDTH,
                  y * TILE_HEIGHT + TILE_HEIGHT - WALL_WIDTH / 2,
                  TILE_WIDTH,
                  WALL_WIDTH);
    }

    // Draw right wall.
    if (!in_bounds(graph, point_after_moving(p, RIGHT)) || !is_connected(tile, RIGHT)) {
        fill_rect(gui, camera,
                  x * TILE_WIDTH + TILE_WIDTH - WALL_WIDTH / 2,
                  y * TILE_HEIGHT,
                  WALL_WIDTH,
                  TILE_HEIGHT);
    }

    // Draw left wall.
    if (!in_bounds(graph, point_after_moving(p, LEFT)) || !is_connected(tile, LEFT)) {
        fill_rect(gui, camera,
                  x * TILE_WIDTH - WALL_WIDTH / 2,
                  y * TILE_HEIGHT,
                  WALL_WIDTH,
                  TILE_HEIGHT);
    }

}



