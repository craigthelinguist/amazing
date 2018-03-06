
#include "collision_map0.h"
#include "graphical_constants.h"
#include <stdio.h>

bool position_is_walkable(struct collision_map *cmap, int16_t x, int16_t y) {
    if (x >= cmap->width || x < 0 || y >= cmap->width || y < 0)
        return false;
    else
        return !cmap->map[y * cmap->width + x];
}

bool is_box_colliding(CollisionMap cmap, SDL_Rect bounding_box) {

    // By custom, things outside the collision_map are considered to be colliding.
    // Check if either of the four corners of the bounding box is outside of the map.
    if (bounding_box.x < 0 || bounding_box.x + bounding_box.w >= cmap->width + MAP_TILE_SZ
        || bounding_box.y < 0 || bounding_box.y + bounding_box.h >= cmap->width + MAP_TILE_SZ)
            return true;

    // The sprite doesn't have to be the same size as the tiles on the cmap, so we iterate over those tiles touched by
    // the bounding box and check if any are solid.
    int start_x, start_y, end_x, end_y;
    start_x = bounding_box.x / MAP_TILE_SZ;
    start_y = bounding_box.y / MAP_TILE_SZ;
    end_x = (bounding_box.x + bounding_box.w) / MAP_TILE_SZ;
    end_y = (bounding_box.x + bounding_box.h) / MAP_TILE_SZ;

    int i, j;
    for (j = start_y; j < end_y; j++) {
        for (i = start_x; i < end_x; i++) {
            if (cmap->map[j * cmap->width + i]) {
                return false;
            }
        }
    }
    return true;

}