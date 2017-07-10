
#ifndef IMAGELIB_H
#define IMAGELIB_H

#include "image.h"
#include <SDL2/SDL.h>

/**
	Initialise this library. Afterwards, you can use imagelib_get and imagelib_load.
**/
void imagelib_init(const int capacity);
	
/**
	Frees all images that have been loaded into this library and
	the library itself.
**/
void imagelib_free();

/**
	Get the image in the library with the given filename. If the
	image does not exist, NULL will be returned.
**/
struct Image *imagelib_get(char *name);

/**
	Load the image with name fname using the specified pixel format.
	The image will be added to the library.
	
	fname: should be a null-teminated string with no more than 15
		characters (excluding the null-terminator).
		
	format: should be the pixel format of the surface on which the
		image shall be drawn.
		
	Returns a non-zero char if loading was successful.
**/
char imagelib_load(char *fname, SDL_Renderer *renderer);
	
#endif
