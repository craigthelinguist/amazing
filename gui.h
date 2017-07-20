

#ifndef GUI_H
#define GUI_H

#include "colors.h"
#include <SDL2/SDL.h>

typedef struct Gui *GUI;

GUI GUI_Make(const int WIDTH, const int HEIGHT);

void GUI_Free(GUI gui);

void GUI_SetBackgroundColor(GUI gui, struct Color colour);

COLOUR GUI_GetBackgroundColor(GUI gui);

SDL_Renderer *GUI_GetRenderer(GUI gui);

const SDL_PixelFormat *GUI_GetScreenFormat(GUI gui);

#endif