
#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

typedef struct entity Entity;

SDL_Rect entity_bbox(struct entity *entity);
SDL_Rect entity_bbox_after_move(struct entity *entity, int dx, int dy);
SDL_Rect entity_pos(struct entity *entity);

#endif
