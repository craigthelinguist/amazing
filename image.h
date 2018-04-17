
#ifndef IMAGE_H
#define IMAGE_H

#include <SDL2/SDL.h>
#include "stdint.h"

typedef struct image image;

typedef struct image_sheet image_sheet;

struct image {
	uint16_t wd;
	uint16_t ht;
	SDL_Texture *texture;
};

/** This is a grid of images, stored in a single file. */
struct image_sheet {

    // The actual file, containing the images.
	image img;

    // The size of each image (width AND height).
	uint16_t img_size;

};

SDL_Rect extract_img(image_sheet *sheet, int index);

SDL_Rect extract_img_rowcol(image_sheet *sheet, int row, int col);

image_sheet make_image_sheet(image image, uint16_t img_size);

#endif
