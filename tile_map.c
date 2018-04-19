
#include "game_state0.h"
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
    if (box.x < 0 || box.x + box.w >= MAP_WD_PIXELS + MAP_TILE_SZ || box.y < 0
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

/// Set the tile_map. We have to figure out which index on the prefab is going to go at this entry in the
/// tile_map. The prefabs are indexed so they correspond directly to the way a node is represented as a
/// binary string in a graph.
void generate_tile_map(map_data *map, graph *graph) {
    tileset_index *tile_map = get_tile_map(map);
    for (int row = 0; row < graph->width; row++) {
        for (int col = 0; col < graph->width; col++) {
            const int TILE_INDEX = row * graph->width + col;
            tileset_index prefab_index = graph->nmap[TILE_INDEX];
            tile_map[TILE_INDEX] = prefab_index;
        }
    }
}



void generate_wall_map(map_data *map, SDL_Surface *wall_map_img) {

    bool *wall_map = get_wall_map(map);


    /*
    // TODO: the bottom loop is incomplete and segfaults at the moment, but the code works (and does nothing) without it

    for (int row = 0; row < graph->width; row++) {
        for (int col = 0; col < graph->width; col++) {


            const int TILE_INDEX = row * graph->width + col;
            tileset_index prefab_index = graph->nmap[TILE_INDEX];
            tile_map[TILE_INDEX] = prefab_index;


            // The (row, col) in the wall_map where we begin iteration.
            const int TILES_PER_PREFAB = (PREFAB_WIDTH / MAP_TILE_SZ) * (PREFAB_WIDTH / MAP_TILE_SZ);
            int pf_row0 = TILES_PER_PREFAB * row;
            int pf_col0 = TILES_PER_PREFAB * col;

            // The top-left (x, y) on the prefab image of the tile corresponding to the (row, col) above.
            // TODO: we need to extract the (row, col) from prefab_index
            // TODO: then turn the (row, col) into the pixel position (x, y)
            int pf_pixel_x0 = 0;
            int pf_pixel_y0 = 0;

            // Iterate over the tiles inside the prefab tile.
            for (int pf_row = pf_row0; pf_row < pf_row0 + TILES_PER_PREFAB; pf_row++) {
                for (int pf_col = pf_col0; pf_col < pf_col0 + TILES_PER_PREFAB; pf_col++) {

                    // Figure out the position on the collision map image for the current wall_map index.
                    int pf_pixel_x_offset = (pf_col - pf_col0) * MAP_TILE_SZ;
                    int pf_pixel_y_offset = (pf_row - pf_row0) * MAP_TILE_SZ;
                    int pf_pixel_x = pf_pixel_x0 + pf_pixel_x_offset + 1;
                    int pf_pixel_y = pf_pixel_y0 + pf_pixel_y_offset + 1;
                    int pixel_index = pf_pixel_y * prefab->w + pf_pixel_x;

                    // Now get the actual RGB values for that pixel.
                    Uint32 *pixels = (Uint32 *)prefab->pixels;
                    Uint8 red, green, blue;
                    SDL_GetRGB(pixels[pixel_index], prefab->format, &red, &green, &blue);

                    // Determine if the pixel is red or green (or bad) and set the wall map appropriately.
                    if (red & !green & !blue) {
                        wall_map[pf_row * map->wall_map_wd + pf_col] = false;
                    } else if (!red & green & !blue) {
                        wall_map[pf_row * map->wall_map_wd + pf_col] = true;
                    } else {
                        fprintf(stderr,
                                "Unknown RGB value (%d, %d, %d) for pixel at (%d, %d) on collision map\n",
                                red, green, blue, pf_pixel_x, pf_pixel_y);
                        exit(23498234);
                    }
                }
            }
        }
    }
    */

}

map_data *generate_map_data(graph *graph, SDL_Renderer *renderer, const char *PREFAB_TILES, const char *PREFAB_WALLS) {

    // Load the prefab tiles.
    if (!imagelib_load(PREFAB_TILES, renderer))
        IMAGELIB_ERR("Could not load the prefab tiles.");

    // Create the filepath for loading the wall_map as an SDL_Surface.
    char *PREFAB_WALLS_FPATH = make_asset_fpath(PREFAB_WALLS);
    if (!PREFAB_WALLS_FPATH) {
        fprintf(stderr, "Generating tilemap but imagelib hasn't been initialised.");
        exit(1204102);
    }

    // Load the wall_map as an SDL_Surface. In this format, we can inspect its pixels. Lock it as well, if necessary.
    SDL_Surface *wall_map_img = IMG_Load(PREFAB_WALLS_FPATH);
    free(PREFAB_WALLS_FPATH);
    if (!wall_map_img) {
        fprintf(stderr, "Failed to load prefab collision map. SDL Error: %s\n", SDL_GetError());
        exit(2354802935);
    }

    if (SDL_MUSTLOCK(wall_map_img))
        SDL_LockSurface(wall_map_img);

    // Create the map using the prefab tiles.
    image_sheet tile_set = make_image_sheet(*imagelib_get(PREFAB_TILES), PREFAB_WIDTH);
    map_data *map = make_map_data(graph->width, tile_set);

    // Initialise the game tile_map and wall_map.
    generate_tile_map(map, graph);
    generate_wall_map(map, wall_map_img);

    // Clean up and return.
    if (SDL_MUSTLOCK(wall_map_img))
        SDL_UnlockSurface(wall_map_img);
    free(wall_map_img);
    return map;

}
