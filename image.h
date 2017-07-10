


#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <SDL2/SDL.h>

struct Image {
	uint16_t wd;
	uint16_t ht;
	SDL_Texture *texture;
};

#endif

