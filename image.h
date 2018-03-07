
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

struct image_sheet {
	image img;
	uint16_t img_size;
};

SDL_Rect extract_img(image_sheet *sheet, int index);

SDL_Rect extract_img_rowcol(image_sheet *sheet, int row, int col);

#endif
