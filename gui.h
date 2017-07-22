

#ifndef GUI_H
#define GUI_H

#include "colours.h"
#include "point.h"
#include <SDL2/SDL.h>

typedef struct Gui *GUI;

GUI make_gui(const uint16_t WIDTH, const uint16_t HEIGHT);

void free_gui(GUI gui);

void set_bgcol(GUI gui, Colour colour);

Colour get_bgcol(GUI gui);

SDL_Renderer *get_renderer(GUI gui);

const SDL_PixelFormat *get_screen_format(GUI gui);

Point_Int32 offset_point(GUI gui, uint32_t x, uint32_t y);
	
#endif