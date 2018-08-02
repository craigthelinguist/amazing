
#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

#include "camera0.h"
#include "colours.h"
#include "point.h"

typedef struct Gui *GUI;

GUI make_gui(const uint16_t WIDTH, const uint16_t HEIGHT);

void free_gui(GUI gui);

void set_bgcol(GUI gui, Colour colour);

Colour get_bgcol(GUI gui);

SDL_Renderer *get_renderer(GUI gui);

const SDL_PixelFormat *get_screen_format(GUI gui);

struct pt_float offset_point(GUI gui, Camera camera, uint32_t x, uint32_t y);

#endif
