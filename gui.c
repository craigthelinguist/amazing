
#include "gui.h"
#include <stdlib.h>

struct Gui {
	SDL_Window *window;
	SDL_Renderer *renderer;
	int screenWidth;
	int screenHeight;
	COLOUR bgColour;
};

struct Gui *GUI_Make(const int WIDTH, const int HEIGHT) {	
	SDL_Window *window = SDL_CreateWindow("Amazing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
		return NULL;
	struct Gui *gui = malloc(sizeof(struct Gui));
	gui->window = window;
	gui->screenWidth = WIDTH;
	gui->screenHeight = HEIGHT;
	gui->renderer = SDL_CreateRenderer(window, -1, 0);
	// Default background colour is white.
	gui->bgColour = (COLOUR) { 255, 255, 255, 255 };
	return gui;
}

const SDL_PixelFormat *GUI_GetScreenFormat(GUI gui) {
	return SDL_GetWindowSurface(gui->window)->format;
}

void GUI_Free(GUI gui) {
	SDL_Surface *surface = SDL_GetWindowSurface(gui->window);
	SDL_FreeSurface(surface);
	surface = NULL;
	SDL_DestroyWindow(gui->window);
	gui->window = NULL;
	SDL_DestroyRenderer(gui->renderer);
	gui->renderer = NULL;
	free(gui);
	gui = NULL;
}

void GUI_SetBackgroundColor(GUI gui, COLOUR colour) {
	gui->bgColour = colour;
}

COLOUR GUI_GetBackgroundColor(GUI gui) {
	return gui->bgColour;
}

SDL_Renderer *GUI_GetRenderer(GUI gui) {
	return gui->renderer;
}

