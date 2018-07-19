
#ifndef TILE_MAP0_H
#define TILE_MAP0_H

#include "graph0.h"
#include "graphical_constants.h"
#include "image.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "tile_map.h"

typedef struct map_data map_data;

typedef TILE tileset_index;

struct tileset {
    struct image_sheet lower;
    struct image_sheet upper;
};

struct map_data {

    // The prefabbed blocks of the maze that fit together.
    struct tileset tileset;

    // The size (in bytes) of the tile_map.
    // The number of entries in the tile_map is tile_map_sz / sizeof(tileset_index)
    int16_t tile_map_sz;

    // The size (in bytes) of the wall_map.
    // The number of entries in the tile_map is wall_map_sz / sizeof(bool)
    // But since sizeof(bool) = 1, this is the same as wall_map_sz
    int16_t wall_map_sz;

    // The width (and height) of the maze in prefab tiles.
    int16_t maze_width_in_prefabs;

    // An array containing (successively) the data about the tiles on the map, and the collision map for them.
    // An entry in the map_data is an index, referencing an image stored on the image_sheet above.
    // An entry in the wall_map is true iff it is a wall iff it is NOT walkable
    char data[];

};

tileset_index *get_tile_map(map_data *map);

bool *get_wall_map(map_data *map);

map_data *make_map_data(int maze_width, struct tileset tileset);

bool is_tile_walkable(map_data *map, int16_t row, int16_t col);

bool is_box_colliding(map_data *map, SDL_Rect box);

map_data *generate_map_data(
        graph *graph,
        SDL_Renderer *renderer,
        const char *PREFAB_LOWER,
        const char *PREFAB_UPPER,
        const char *PREFAB_WALLS
);

#endif
