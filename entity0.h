
#ifndef ENTITY0_H
#define ENTITY0_H

#include "entity.h"
#include "sprite.h"

#define ENTITY_BASE_WIDTH 16
#define ENTITY_BASE_HEIGHT 16

struct entity {
    float xpos;
    float ypos;
    uint8_t collision_wd;
    uint8_t collision_ht;
    Sprite sprite;
};

#endif
