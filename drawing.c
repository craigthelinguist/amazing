


#include <stdio.h>

#include <SDL_image.h>

#include "colours.h"
#include "drawing.h"
#include "gui.h"
#include "point.h"

void draw_image(GUI gui, Camera camera, image *image, int x, int y) {
	Point_Int32 pt = offset_point(gui, camera, x, y);
	SDL_Rect rect = {pt.x, pt.y, image->wd, image->ht};
	SDL_RenderCopy(get_renderer(gui), image->texture, NULL, &rect);
	
	/*
	SDL_Rect rect = {x, y, image->wd, image->ht};
	SDL_RenderCopy(get_renderer(gui), image->texture, NULL, &rect);
	// Use this for drawing surfaces.
	// SDL_BlitSurface(image, NULL, SDL_GetWindowSurface(gui->window), NULL);
	*/
}

void draw_image_offset(GUI gui, Camera camera, image *image, int x, int y, int off_x, int off_y, int wd, int ht) {

	// Select the part of the texture to be rendered.
	SDL_Rect text_rect = {off_x, off_y, wd, ht};

	// Compute the position on the screen where it should be drawn.
	Point_Int32 pt = offset_point(gui, camera, x, y);
	SDL_Rect dest_rect = {pt.x, pt.y, wd, ht};

	SDL_RenderCopy(get_renderer(gui), image->texture, &text_rect, &dest_rect);

}



void draw_rect(GUI gui, Camera camera, const int64_t x, const int64_t y, const int64_t wd, const int64_t ht) {
	Point_Int32 pt = offset_point(gui, camera, x, y);
	SDL_Rect rect = {pt.x, pt.y, wd, ht};
	SDL_RenderDrawRect(get_renderer(gui), &rect);
	/*
	SDL_Rect rect = {x, y, wd, ht};
	SDL_RenderDrawRect(get_renderer(gui), &rect);*/
}

void fill_rect(GUI gui, Camera camera, const int64_t x, const int64_t y, const int64_t wd, const int64_t ht) {
	Point_Int32 pt = offset_point(gui, camera, x, y);
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
