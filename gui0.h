

#ifndef GUI0_H
#define GUI0_H

#include <SDL2/SDL.h>

#include "camera0.h"
#include "colours.h"
#include "gui.h"
#include "point.h"

struct Gui {
    SDL_Window *window;
    SDL_Renderer *renderer;
    uint16_t screen_wd;
    uint16_t screen_ht;
    Colour bgcol;
};

#endif