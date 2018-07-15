

#include "entity0.h"
#include "graphical_constants.h"
#include "sprite0.h"

SDL_Rect entity_bbox(struct entity *entity) {
    return (SDL_Rect) {
        entity->xpos,
        entity->ypos + entity->sprite->ht - entity->collision_ht,
        entity->collision_wd,
        entity->collision_ht,
    };
}

SDL_Rect entity_bbox_after_move(struct entity *entity, int dx, int dy) {
    return (SDL_Rect) {
        entity->xpos + dx,
        entity->ypos + entity->sprite->ht - entity->collision_ht + dy,
        entity->collision_wd,
        entity->collision_ht
    };
}

SDL_Rect entity_pos(struct entity *entity) {
    return (SDL_Rect) {
        entity->xpos,
        entity->ypos,
        entity->sprite->wd,
        entity->sprite->ht,
    };
}

