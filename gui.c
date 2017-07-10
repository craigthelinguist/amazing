

#include "colors.h"
#include "gui.h"
#include <SDL_image.h>
#include <stdio.h>

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

void GUI_DrawImageToScreen(GUI gui, struct Image *image, int x, int y) {
	SDL_Rect rect = {x, y, image->wd, image->ht};
	SDL_RenderCopy(gui->renderer, image->texture, NULL, &rect);
	// Use this for drawing surfaces.
	// SDL_BlitSurface(image, NULL, SDL_GetWindowSurface(gui->window), NULL);
}

void GUI_DrawRect(GUI gui, const int64_t x, const int64_t y, const int64_t wd, const int64_t ht) {
	SDL_Rect rect = {x, y, wd, ht};
	SDL_RenderDrawRect(gui->renderer, &rect);
}

void GUI_FillRect(GUI gui, const int64_t x, const int64_t y, const int64_t wd, const int64_t ht) {
	SDL_Rect rect = {x, y, wd, ht};
	SDL_RenderFillRect(gui->renderer, &rect);
}
						
							
void GUI_ClearScreen(GUI gui) {
	GUI_SetColor2(gui, gui->bgColour);
	SDL_RenderClear(gui->renderer);
}

void GUI_RefreshScreen(GUI gui) {
	SDL_RenderPresent(gui->renderer);
}

SDL_Renderer *GUI_GetRenderer(GUI gui) {
	return gui->renderer;
}

void GUI_SetBackgroundColor(GUI gui, COLOUR colour) {
	gui->bgColour = colour;
}

void GUI_SetColor(GUI gui, const int8_t r, const int8_t g, const int8_t b, const int8_t alpha) {
	SDL_SetRenderDrawColor(gui->renderer, r, g, b, alpha);
}

void GUI_SetColor2(GUI gui, struct Color colour) {
	SDL_SetRenderDrawColor(gui->renderer, colour.red, colour.green, colour.blue, colour.alpha);
}

