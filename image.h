


#ifndef IMAGE_H
#define IMAGE_H

typedef struct Image Image;

struct Image {
	uint16_t wd;
	uint16_t ht;
	SDL_Texture *texture;
};

#endif

