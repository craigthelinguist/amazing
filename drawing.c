

#include "colors.h"
#include "drawing.h"
#include "gui.h"
#include <SDL_image.h>
#include <stdio.h>

void GUI_DrawImageToScreen(GUI gui, struct Image *image, int x, int y) {
	SDL_Rect rect = {x, y, image->wd, image->ht};
	SDL_RenderCopy(GUI_GetRenderer(gui), image->texture, NULL, &rect);
	// Use this for drawing surfaces.
	// SDL_BlitSurface(image, NULL, SDL_GetWindowSurface(gui->window), NULL);
}

void GUI_DrawRect(GUI gui, const int64_t x, const int64_t y, const int64_t wd, const int64_t ht) {
	SDL_Rect rect = {x, y, wd, ht};
	SDL_RenderDrawRect(GUI_GetRenderer(gui), &rect);
}

void GUI_FillRect(GUI gui, const int64_t x, const int64_t y, const int64_t wd, const int64_t ht) {
	SDL_Rect rect = {x, y, wd, ht};
	SDL_RenderFillRect(GUI_GetRenderer(gui), &rect);
}


void GUI_ClearScreen(GUI gui) {
	GUI_SetColor2(gui, GUI_GetBackgroundColor(gui));
	SDL_RenderClear(GUI_GetRenderer(gui));
}

void GUI_RefreshScreen(GUI gui) {
	SDL_RenderPresent(GUI_GetRenderer(gui));
}


void GUI_SetColor(GUI gui, const int8_t r, const int8_t g, const int8_t b, const int8_t alpha) {
	SDL_SetRenderDrawColor(GUI_GetRenderer(gui), r, g, b, alpha);
}

void GUI_SetColor2(GUI gui, struct Color colour) {
	SDL_SetRenderDrawColor(GUI_GetRenderer(gui), colour.red, colour.green, colour.blue, colour.alpha);
}

