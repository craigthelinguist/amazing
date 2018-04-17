
#ifndef TILE_MAP0_H
#define TILE_MAP0_H

#include "graph0.h"
#include "graphical_constants.h"
#include "image.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "tile_map.h"

typedef struct map_data map_data;

typedef TILE tileset_index;

struct map_data {

    // The prefabbed blocks of the maze that fit together.
    image_sheet tile_set;

    // The width (and height) of the tile map. Will be the number of prefab blocks that fit into the map.
    int16_t tile_map_wd;

    // The width (and height) of the wall map. Will be the same as the graphical constant.
    int16_t wall_map_wd;

    // An array containing (successively) the data about the tiles on the map, and the collision map for them.
    // An entry in the map_data is an index, referencing an image stored on the image_sheet above.
    // An entry in the wall_map is true iff it is a wall iff it is NOT walkable
    char *data;

};

tileset_index *get_tile_map(map_data *map);

bool *get_wall_map(map_data *map);

map_data *make_map_data(int maze_width, image_sheet prefabs);

bool is_tile_walkable(map_data *map, int16_t row, int16_t col);

bool is_box_colliding(map_data *map, SDL_Rect box);

map_data *generate_map_data(graph *graph, image_sheet tile_set);

#endif
