
#ifndef ENTITY0_H
#define ENTITY0_H

#include "entity.h"
#include "image.h"

#define ENTITY_BASE_WIDTH 16
#define ENTITY_BASE_HEIGHT 16

struct Entity {
    int32_t xpos;
    int32_t ypos;
    Image *image;
};

#endif
