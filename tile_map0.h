
#ifndef TILE_MAP0_H
#define TILE_MAP0_H

#include "graphical_constants.h"
#include "image.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "tile_map.h"

struct tile_map {
    image *tileset;
    int16_t width;
    int16_t map[];
};

TileMap make_tile_map(int16_t width, image *tileset) {

    // The tileset width/height should be a multiple of the tile size.
    if (!tileset || tileset->wd % MAP_TILE_SZ != 0 || tileset->ht % MAP_TILE_SZ != 0)
        return NULL;

    struct tile_map *tm = malloc(sizeof(struct tile_map) + sizeof(int16_t) * width * width);
    tm->width = width;
    memset(tm->map, -1, width * width);
    tm->tileset = tileset;
    return tm;
}

void set_tile(struct tile_map *tm, int16_t x, int16_t y, int16_t tile_index) {

    // Check the tile_index is in bounds.
    const int NUM_TILES = (tm->tileset->ht / MAP_TILE_SZ) * (tm->tileset->wd / MAP_TILE_SZ);
    if (tile_index < 0 || tile_index >= NUM_TILES) {
        fprintf(stderr, "Setting tile image which is out of bounds (index %d)\n", tile_index);
        exit(1350);
    }

    // Check the map position is in bounds.
    if (x < 0 || y < 0 || x >= tm->width || y >= tm->width) {
        fprintf(stderr, "Setting tile which is not located on the map (%d, %d)\n", x, y);
        exit(1351);
    }

    tm->map[y * tm->width + x] = tile_index;

}

#endif
