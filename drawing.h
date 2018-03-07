
#ifndef DRAWING_H
#define DRAWING_H

#include "colours.h"
#include "gui.h"
#include "image.h"
#include <SDL2/SDL.h>

void draw_image(GUI gui, Camera camera, image *image, int x, int y);

void draw_image_offset(GUI gui, Camera camera, image *image, int x, int y, int off_x, int off_y, int wd, int ht);

void clear_screen(GUI gui);

void refresh_screen(GUI gui);

void draw_rect(GUI gui, Camera camera,
               const int64_t x, const int64_t y, const int64_t wd, const int64_t ht);

void fill_rect(GUI gui, Camera camera,
               const int64_t x, const int64_t y, const int64_t wd, const int64_t ht);

void set_drawcol(GUI gui, const int8_t r, const int8_t g, const int8_t b, const int8_t alpha);

void set_drawcol2(GUI gui, Colour colour);

#endif

