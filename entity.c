

#include "entity0.h"
#include "graphical_constants.h"
#include "sprite0.h"

SDL_Rect entity_bbox(struct entity *entity) {
    return (SDL_Rect) {
        entity->xpos,
        entity->ypos + SPRITE_HT - SPRITE_COLLISION_HT,
        SPRITE_COLLISION_WD,
        SPRITE_COLLISION_HT
    };
}

SDL_Rect entity_bbox_after_move(struct entity *entity, int dx, int dy) {
    return (SDL_Rect) {
        entity->xpos + dx,
        entity->ypos + dy + SPRITE_HT - SPRITE_COLLISION_HT,
        SPRITE_COLLISION_WD,
        SPRITE_COLLISION_HT
    };
}

SDL_Rect entity_pos(struct entity *entity) {
    return (SDL_Rect) {
        entity->xpos,
        entity->ypos,
        SPRITE_WD,
        SPRITE_HT
    };
}

