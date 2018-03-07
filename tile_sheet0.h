
#ifndef IMAGE_SHEET0_H
#define IMAGE_SHEET0_H

#include "tile_sheet.h"
#include "image.h"
#include <stdint.h>

struct image_sheet {
    Image *image;
    uint16_t tile_size;

};

#endif
