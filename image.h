


#ifndef IMAGE_H
#define IMAGE_H

struct Image {
	uint16_t wd;
	uint16_t ht;
	SDL_Texture *texture;
};

typedef struct Image Image;

#endif

