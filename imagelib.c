
#include "gui.h"
#include "imagelib.h"
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define STRING_SIZE 32

struct Entry {
	struct Image image;
	char fname[STRING_SIZE];
};

struct ImageLib {
	int capacity;
	int size;
	struct Entry *data;
};

static struct ImageLib lib;

char streq(const char *s1, const char *s2) {
	return !strcmp(s1, s2);
}

void imagelib_init(const int maxSize) {
	lib.data = calloc(sizeof(struct Entry), maxSize);
	lib.capacity = maxSize;
	lib.size = 0;
}

void imagelib_free() {
	for (int i = 0; i < lib.capacity || lib.data + i != NULL; i++) {
		SDL_DestroyTexture(lib.data[i].image.texture);
	}
	free(lib.data);
}

char imagelib_load(char *fname, SDL_Renderer *renderer) {
	
	// Fail if name of file is too long.
	if (strlen(fname) > STRING_SIZE - 1) return 0;
	
	// Fail if library is at full capacity.
	if (lib.size == lib.capacity) return 0;
	
	// Load the image in fname as an SDL_Surface.
	SDL_Surface *surface = IMG_Load(fname);
	if (surface == NULL) return 0;

	// Create a texture from the image.
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);	
	if (!texture) {
		free(surface);
		return 0;
	}
	
	// Store the texture and image data.
	lib.data[lib.size].image.wd = surface->w;
	lib.data[lib.size].image.ht = surface->h;
	lib.data[lib.size].image.texture = texture;

	// Remember the filepath associated with this texture.
	strcpy(lib.data[lib.size].fname, fname);

	// Clean up, increment size.
	lib.size++;
	SDL_FreeSurface(surface);
	return 1;
	
}

struct Image *imagelib_get(char *fname) {
	for (int i = 0; i < lib.size; i++) {
		if (!strcmp(fname, lib.data[i].fname))
			return &(lib.data[i].image);
	}
	return NULL;
}


int main(int argc, char *argv[]) {

	// Initialise SDL and the GUI.
	SDL_Init(SDL_INIT_VIDEO);
	GUI gui = GUI_Make(800, 400);

	// Initialise imagelib.
	imagelib_init(3);
	
	SDL_Renderer *r = GUI_GetRenderer(gui);
	imagelib_load("assets/maria.png", r);
	imagelib_load("assets/relm.png", r);
	imagelib_load("assets/leo.png", r);
	
	GUI_ClearScreen(gui);
	GUI_DrawImageToScreen(gui, imagelib_get("assets/maria.png"), 0, 0);
	GUI_DrawRect(gui, 10, 10, 20, 20);
	GUI_RefreshScreen(gui);
	SDL_Delay(2000);
	
	GUI_ClearScreen(gui);
	GUI_DrawImageToScreen(gui, imagelib_get("assets/relm.png"), 0, 0);        
	GUI_FillRect(gui, 50, 50, 20, 20);
	GUI_RefreshScreen(gui);
	
	SDL_Delay(2000);
	
	GUI_ClearScreen(gui);
	GUI_DrawImageToScreen(gui, imagelib_get("assets/leo.png"), 0, 0);     
	GUI_FillRect(gui, 50, 50, 20, 20);
	GUI_RefreshScreen(gui);
				/*
				
	
	imagelib_load("assets/maria.png", gui);

	// Draw the image.
	SDL_Surface *img = imagelib_get("assets/maria.png");
	if (img == NULL)
		printf("wtf happened\n");
	
	GUI_ClearScreen(gui);
	GUI_DrawImageToScreen(gui, imagelib_get("assets/maria.png"));

	GUI_DrawRect(gui, 50, 50, 20, 20);
	GUI_RefreshScreen(gui);
	

	
	SDL_Delay(2000);
	
	GUI_ClearScreen(gui);
	GUI_DrawImageToScreen(gui, imagelib_get("assets/relm.png"));        
	GUI_FillRect(gui, 50, 50, 20, 20);
	GUI_RefreshScreen(gui);
	
	SDL_Delay(2000);
	
	GUI_ClearScreen(gui);
	GUI_DrawImageToScreen(gui, imagelib_get("assets/leo.png"));     
	GUI_FillRect(gui, 50, 50, 20, 20);
	GUI_RefreshScreen(gui);
	
	SDL_Delay(2000);
	*/
	return 0;
	
}




