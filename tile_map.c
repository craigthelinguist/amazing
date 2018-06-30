
#include "game_state0.h"
#include "graph0.h"
#include "imagelib.h"
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "tile_map0.h"

void debug_wall_map(FILE *fp, map_data *map);

tileset_index *get_tile_map(map_data *map) {
    return (tileset_index *) &map->data;
}

bool *get_wall_map(map_data *map) {
    // Skip over the tile_map and get a pointer to the start of the wall_map.
    char *ptr = (char *) map->data;
    return (bool *) (ptr + map->tile_map_sz);
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

    // Figure out how big is the array.
    const int TILES_PER_PREFAB_BLOCK = (PREFAB_WIDTH / MAP_TILE_SZ) * (PREFAB_WIDTH / MAP_TILE_SZ);
    const int TILE_MAP_SIZE = (maze_width * maze_width) * sizeof(tileset_index);
    const int WALL_MAP_SIZE = (TILES_PER_PREFAB_BLOCK * maze_width * maze_width) * sizeof(bool);
    const int ARRAY_SIZE = TILE_MAP_SIZE + WALL_MAP_SIZE;

    // Allocate the map.
    map_data *map = malloc(sizeof(map_data) + TILE_MAP_SIZE + WALL_MAP_SIZE);
    if (!map) {
        fprintf(stderr, "make_map_data: failed to malloc.");
        exit(123523);
    }

    // Initialise the map_data.
    map->tile_set = prefabs;
    map->tile_map_sz = TILE_MAP_SIZE;
    map->wall_map_sz = WALL_MAP_SIZE;
    map->maze_width_in_prefabs = maze_width;
    memset(&map->data, 0, TILE_MAP_SIZE + WALL_MAP_SIZE);

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
    if (col >= map->wall_map_sz || col < 0 || row >= map->wall_map_sz || row < 0)
        return false;
    bool *wall_map = get_wall_map(map);
    int skip_ahead = (row * map->wall_map_sz) + col;
    return wall_map + skip_ahead;
}

bool is_out_of_bounds(map_data *map, SDL_Rect box) {
    const int MAP_WD = map->maze_width_in_prefabs * PREFAB_WIDTH;
    if (box.x < 0 || box.y < 0) {
        return true;
    } else if (box.x + box.w >= MAP_WD) {
        return true;
    } else if (box.y + box.h >= MAP_WD) {
        return true;
    } else {
        return false;
    }
}

bool is_box_colliding(map_data *map, SDL_Rect box) {

    // Each row of the wall_map contains this many tiles.
    const int TILES_PER_ROW = (PREFAB_WIDTH / MAP_TILE_SZ) * map->maze_width_in_prefabs;

    // By custom, anything out of bounds is considered a "wall".
    // So let's check if any of the four corners of the box are out of bounds.
    if (is_out_of_bounds(map, box)) {
        return true;
    }

    // Iterate over all the tiles inside the box and check if any of them is a wall.
    int start_x, start_y, end_x, end_y;
    start_x = box.x / MAP_TILE_SZ;
    start_y = box.y / MAP_TILE_SZ;
    end_x = (box.x + box.w) / MAP_TILE_SZ;
    end_y = (box.y + box.h) / MAP_TILE_SZ;
    bool *wall_map = get_wall_map(map);
    for (int row = start_y; row < end_y; row++) {
        for (int col = start_x; col < end_x; col++) {
            bool touching_wall = wall_map[row * TILES_PER_ROW + col];
            if (touching_wall) {
                return true;
            }
        }
    }

    return false;
}

/// Set the tile_map. We have to figure out which index on the prefab is going to go at this entry in the
/// tile_map. The prefabs are indexed so they correspond directly to the way a node is represented as a
/// binary string in a graph, so we can just directly copy the graph's nmap.
void generate_tile_map(map_data *map, graph *graph) {
    tileset_index *tile_map = get_tile_map(map);
    memcpy(tile_map, graph->nmap, sizeof(tileset_index) * graph->width * graph->width);
}



#define LOG(...) \
    do { FILE *f = fopen("amazing.log", "a+"); fprintf(f, __VA_ARGS__); fclose(f); } while (0);



void generate_wall_map(map_data *map, SDL_Surface *wall_map_img) {

    // These are the maps we're going to be working with.
    bool *wall_map = get_wall_map(map);
    tileset_index *tile_map = get_tile_map(map);

    // This is the width (and height) of the map in prefabs.
    const int MAZE_WD_PREFABS = map->maze_width_in_prefabs;

    // This is the amount of regular tiles in a single prefab.
    const int PREFAB_WD_IN_TILES = PREFAB_WIDTH / MAP_TILE_SZ;
    const int TILES_PER_PREFAB = PREFAB_WD_IN_TILES * PREFAB_WD_IN_TILES;

    // Read from the wall map image and load the data into the wall_map.
    Uint32 *pixels = (Uint32 *)wall_map_img->pixels;

    for (int row = 0; row < MAZE_WD_PREFABS; row++) {
        for (int col = 0; col < MAZE_WD_PREFABS; col++) {

            // This is the index of the prefab.
            const int PF_TILE_INDEX = tile_map[row * MAZE_WD_PREFABS + col];
            const int WALL_MAP_WD = MAZE_WD_PREFABS * (PREFAB_WIDTH / MAP_TILE_SZ);

            // The top-left (row, col) in the wall_map where we begin iteration and its index in the wall_map.
            int pf_row0 = PREFAB_WD_IN_TILES * row;
            int pf_col0 = PREFAB_WD_IN_TILES * col;
            int pf_index = pf_row0 * WALL_MAP_WD + pf_col0;

            // The top-leftmost pixel of that position.
            int pf_tile_row, pf_tile_col;
            extract_rowcol_from_index(&map->tile_set, PF_TILE_INDEX, &pf_tile_row, &pf_tile_col);

            int pf_pixel_x0 = pf_tile_col * PREFAB_WIDTH;
            int pf_pixel_y0 = pf_tile_row * PREFAB_WIDTH;

            // Iterate over the tiles inside the prefab.
            for (int row2 = 0; row2 < PREFAB_WD_IN_TILES; row2++) {
                for (int col2 = 0; col2 < PREFAB_WD_IN_TILES; col2++) {

                    // Get the position of the current tile.
                    int pf_row = pf_row0 + row2;
                    int pf_col = pf_col0 + col2;
                    int pf_index = pf_row * WALL_MAP_WD + pf_col;

                    // Get a pixel position inside the prefab tile.
                    int pf_pixel_x = pf_pixel_x0 + col2 * MAP_TILE_SZ + 1;
                    int pf_pixel_y = pf_pixel_y0 + row2 * MAP_TILE_SZ + 1;
                    int pf_pixel_index = pf_pixel_y * wall_map_img->w + pf_pixel_x;

                    // Get actual RGB values for pixel and set the appropriate entry in the wall_map.
                    Uint8 red, green, blue;
                    SDL_GetRGB(pixels[pf_pixel_index], wall_map_img->format, &red, &green, &blue);

                    if (red && !green && !blue) {
                        wall_map[pf_index] = true;
                    } else if (!red && green && !blue) {
                        wall_map[pf_index] = false;
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

    debug_wall_map(stdout, map);

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

/*
 * DEBUG: prints the wall_map to a file. */
void debug_wall_map(FILE *out, map_data *map) {

    bool *wall_map = get_wall_map(map);

    for (int i = 0; i < map->wall_map_sz; i++) {
        if (i % ((PREFAB_WIDTH / MAP_TILE_SZ) * map->maze_width_in_prefabs) == 0)
            fprintf(out, "\n");
        if (map->wall_map_sz) {
            fprintf(out, "%d", wall_map[i]);
        }
    }


}

