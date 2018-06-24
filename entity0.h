
#ifndef ENTITY0_H
#define ENTITY0_H

#include "entity.h"
#include "sprite.h"

#define ENTITY_BASE_WIDTH 16
#define ENTITY_BASE_HEIGHT 16

struct entity {
    int32_t xpos;
    int32_t ypos;
    Sprite sprite;
};

#endif
