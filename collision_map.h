
#ifndef COLLISION_MAP
#define COLLISION_MAP

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct collision_map *CollisionMap;

bool is_box_colliding(CollisionMap cmap, SDL_Rect bounding_box);

#endif
