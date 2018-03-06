
#ifndef COLLISION_MAP0_H
#define COLLISION_MAP0_H

#include "collision_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Records where there are tiles on the map. If map[i], then tile i is not walkable.
struct collision_map {
    int16_t width;
    bool map[];
};

CollisionMap make_collision_map(int16_t width) {
    struct collision_map *cmap = malloc(sizeof(int16_t) + sizeof(bool) * width * width);
    cmap->width = width;
    memset(cmap->map, 0, width * width);
    return cmap;
}

void set_tile_walkability(struct collision_map *cmap, int16_t x, int16_t y, bool walkable) {
    if (x >= cmap->width || x < 0 || y >= cmap->width || y < 0) {
        printf("Setting out of bounds collision map tile (%d, %d) ", x, y);
        exit(1500);
    }
    cmap->map[y * cmap->width + x] = !walkable;
}

/* Check if a tile on the map is walkable or not. For efficiency, no bounds checks are done. */
bool is_tile_walkable(struct collision_map *cmap, int16_t x, int16_t y) {
    return !cmap->map[y * cmap->width + x];
}

#endif

