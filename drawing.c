

#include "colours.h"
#include "drawing.h"
#include "gui.h"
#include "point.h"
#include <SDL_image.h>
#include <stdio.h>

void draw_image(GUI gui, struct Image *image, int x, int y) {
	Point_Int32 pt = offset_point(gui, x, y);
	SDL_Rect rect = {pt.x, pt.y, image->wd, image->ht};
	SDL_RenderCopy(get_renderer(gui), image->texture, NULL, &rect);
	
	/*
	SDL_Rect rect = {x, y, image->wd, image->ht};
	SDL_RenderCopy(get_renderer(gui), image->texture, NULL, &rect);
	// Use this for drawing surfaces.
	// SDL_BlitSurface(image, NULL, SDL_GetWindowSurface(gui->window), NULL);
	*/
}

void draw_rect(GUI gui, const int64_t x, const int64_t y, const int64_t wd, const int64_t ht) {
	Point_Int32 pt = offset_point(gui, x, y);
	SDL_Rect rect = {pt.x, pt.y, wd, ht};
	SDL_RenderDrawRect(get_renderer(gui), &rect);
	/*
	SDL_Rect rect = {x, y, wd, ht};
	SDL_RenderDrawRect(get_renderer(gui), &rect);*/
}

void fill_rect(GUI gui, const int64_t x, const int64_t y, const int64_t wd, const int64_t ht) {
	Point_Int32 pt = offset_point(gui, x, y);
	SDL_Rect rect = {pt.x, pt.y, wd, ht};
	SDL_RenderFillRect(get_renderer(gui), &rect);
	/*
	SDL_Rect rect = {x, y, wd, ht};
	SDL_RenderFillRect(get_renderer(gui), &rect);*/
}

void clear_screen(GUI gui) {
	set_drawcol2(gui, get_bgcol(gui));
	SDL_RenderClear(get_renderer(gui));
}

void refresh_screen(GUI gui) {
	SDL_RenderPresent(get_renderer(gui));
}

void set_drawcol(GUI gui, const int8_t r, const int8_t g, const int8_t b, const int8_t alpha) {
	SDL_SetRenderDrawColor(get_renderer(gui), r, g, b, alpha);
}

void set_drawcol2(GUI gui, Colour colour) {
	SDL_SetRenderDrawColor(get_renderer(gui), colour.red, colour.green, colour.blue, colour.alpha);
}
