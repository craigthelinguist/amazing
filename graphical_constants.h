
#ifndef GRAPHICAL_CONSTANTS
#define GRAPHICAL_CONSTANTS

// This is the smallest width/height of a tile on the tile_map. The tileset will be indexed according to this.
// For now, a tile is square.
#define MAP_TILE_SZ 16

// This is the size of a maze prefab. A prefab is like a collection of tiles. Prefabs are placed together to form new,
// novel mazes. For now, a prefab is square.
#define PREFAB_WIDTH 272

// This is the size of a sprite's image in total.
#define SPRITE_WD 32
#define SPRITE_HT 48

// This is the part of the sprite which has a collision box, starting from the bottom-left.
#define SPRITE_COLLISION_WD 32
#define SPRITE_COLLISION_HT 32

#endif
