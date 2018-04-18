
#include "graph0.h"
#include "imagelib.h"
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "tile_map0.h"

tileset_index *get_tile_map(map_data *map) {
    return (tileset_index *) &map->data;
}

bool *get_wall_map(map_data *map) {
    const int num_tiles = map->tile_map_wd * map->tile_map_wd;
    tileset_index *tile_map = (tileset_index *) map->data;
    tile_map += num_tiles;
    return (bool *) tile_map;
}

map_data *make_map_data(int maze_width, image_sheet prefabs) {

    // Do a bunch of sanity checks.
    if (prefabs.img_size != PREFAB_WIDTH) {
        fprintf(stderr, "make_map_data: prefab width is wrong. Should be %d, but it's %d\n",
                PREFAB_WIDTH, prefabs.img_size);
        exit(123518);
    }

    if (PREFAB_WIDTH % MAP_TILE_SZ != 0) {
        fprintf(stderr, "make_map_data: prefab width %d should be multiple of map tile size %d\n",
                PREFAB_WIDTH, MAP_TILE_SZ);
        exit(123519);
    }

    if (prefabs.img.wd % prefabs.img_size != 0 || prefabs.img.ht % prefabs.img_size != 0) {
        fprintf(stderr, "make_map_data: size of prefab not a multiple of size of prefab block.");
        exit(123520);
    }

    if (maze_width <= 0) {
        fprintf(stderr, "make_map_data: width of wall map must be > 0");
        exit(123522);
    }

    fprintf(stderr, "Successfully done sanity checks.\n");

    // Figure out how many entries in the tile_map and the wall_map.
    const int TILES_PER_PREFAB_BLOCK = (PREFAB_WIDTH / MAP_TILE_SZ) * (PREFAB_WIDTH / MAP_TILE_SZ);
    const int TILE_MAP_SIZE = (maze_width * maze_width) * sizeof(tileset_index);
    const int WALL_MAP_SIZE = (TILES_PER_PREFAB_BLOCK * maze_width * maze_width) * sizeof(bool);

    // Based on that, compute the size of the tile_Map and the wall_map and how much memory to allocate.
    const int TILE_MAP_DATA_SIZE = TILE_MAP_SIZE * sizeof(tileset_index);
    const int WALL_MAP_DATA_SIZE = WALL_MAP_SIZE * sizeof(bool);
    const int MALLOC_SIZE = sizeof(map_data) + TILE_MAP_DATA_SIZE + WALL_MAP_DATA_SIZE;

    // Allocate memory for the map.
    map_data *map = malloc(MALLOC_SIZE);
    if (!map) {
        fprintf(stderr, "make_map_data: failed to malloc.");
        exit(123523);
    }

    // Initialise the map_data.
    map->tile_set = prefabs;
    map->tile_map_wd = TILE_MAP_SIZE;
    map->wall_map_wd = WALL_MAP_SIZE;
    memset(&map->data, 0, TILE_MAP_DATA_SIZE + WALL_MAP_DATA_SIZE);
    return map;
}

/*
tile_map make_tile_map(int16_t width, image *tile_set, image *wall_map) {

    // The width/height of the images must be a multiple of the MAP_TILE_SZ.
    if (!tile_set || tile_set->wd % MAP_TILE_SZ != 0 || tile_set->ht % MAP_TILE_SZ != 0)
        return NULL;
    if (!wall_map || wall_map->wd % MAP_TILE_SZ != 0 || wall_map->ht % MAP_TILE_SZ != 0)
        return NULL;

}
 */

bool is_tile_walkable(map_data *map, int16_t row, int16_t col) {
    if (col >= map->wall_map_wd || col < 0 || row >= map->wall_map_wd || row < 0)
        return false;
    bool *wall_map = get_wall_map(map);
    int skip_ahead = (row * map->wall_map_wd) + col;
    return wall_map + skip_ahead;
}

bool is_box_colliding(map_data *map, SDL_Rect box) {

    // Let's pull out some map parameters for easier reference.
    const int MAP_WD = map->wall_map_wd;
    const int MAP_WD_PIXELS = MAP_WD * MAP_TILE_SZ;

    // By custom, anything out of bounds is considered a "wall".
    // So let's check if any of the four corners of the box are out of bounds.
    if  (box.x < 0 || box.x + box.w >= MAP_WD_PIXELS + MAP_TILE_SZ || box.y < 0
         || box.y + box.h >= MAP_WD_PIXELS + MAP_TILE_SZ)
            return true;

    // The box is colliding if it touches any tile which is a wall. We don't presume a uniform sprite size, so we have
    // to iterate over the tile-sized chunks inside the box.
    int start_x, start_y, end_x, end_y;
    start_x = box.x / MAP_TILE_SZ;
    start_y = box.y / MAP_TILE_SZ;
    end_x = (box.x + box.w) / MAP_TILE_SZ;
    end_y = (box.y + box.h) / MAP_TILE_SZ;

    // Iterate over tiles inside the box and check them in the wall map.
    bool *wall_map = get_wall_map(map);
    int row, col;
    for (row = start_y; row < end_y; row++) {
        for (col = start_x; col < end_x; col++) {
            const int skip_ahead = (row * map->wall_map_wd) + col;
            if (*(wall_map + skip_ahead)) {
                return false;
            }
        }
    }
    return true;

}

map_data *generate_map_data(graph *graph, image_sheet tile_set) {

    // Get the tile maps.
    map_data *map = make_map_data(graph->width, tile_set);
    tileset_index *tile_map = get_tile_map(map);
    bool *wall_map = get_wall_map(map);

    // Create the filepath for loading the prefab.
    char *PREFAB_FPATH = make_asset_fpath(FNAME_PREFAB_PATHING);
    if (!PREFAB_FPATH) {
        fprintf(stderr, "Generating tile map but image library hasn't been initialised.");
        exit(104821);
    }

    // Load the actual prefab.
    SDL_Surface *prefab = IMG_Load(PREFAB_FPATH);
    if (!prefab) {
        fprintf(stderr, "Failed to load prefab collision map. SDL Error: %s\n", SDL_GetError());
        free(PREFAB_FPATH);
        exit(2354802935);
    }

    // TODO: Now fill up the tile_map and wall_map using info from the graph and the RGB values
    // TODO: on the prefab image.

    /*
    for (int row = 0; row < graph->width; row++) {
        for (int col = 0; col < graph->width; col++) {
            TILE tile = graph->nmap[row * graph->width + col];
            int index = row * graph->width + col;
            printf("trying to access entry at index %d, which is at position %d\n",
                   index,
                   tile_map + index);

            // Set the tile map.
            tile_map[row * graph->width + col] = tile;

            // Set the collision map. This is a bit harder. We have to figure out which prefab we're looking at, then
            // copy across the tiles from the collision map. To figure that out, we need to check the colour values of
            // tiles on the prefab wall_map image.
            const int TILES_PER_PREFAB = PREFAB_WIDTH / MAP_TILE_SZ;
            int row2 = TILES_PER_PREFAB * row;
            int col2 = TILES_PER_PREFAB * col;

            for (int r = row2; r < PREFAB_WIDTH; r++) {
                for (int c = col2; c < PREFAB_WIDTH; c++) {
                    // wall_map[row2 * map->wall_map_wd + col2] =
                }
            }

        }
    }
    */

    // Free the prefab image and loading apparatus.
    free(PREFAB_FPATH);
    free(prefab);

    return map;

}
