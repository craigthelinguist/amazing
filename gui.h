
#ifndef GUI_H
#define GUI_H

#include "colors.h"
#include "image.h"
#include <SDL2/SDL.h>

typedef struct Gui *GUI;


GUI GUI_Make(const int WIDTH, const int HEIGHT);

const SDL_PixelFormat *GUI_GetScreenFormat(GUI gui);
	
void GUI_DrawImageToScreen(GUI gui, struct Image *image, int x, int y);

void GUI_ClearScreen(GUI gui);

void GUI_RefreshScreen(GUI gui);

void GUI_DrawRect(GUI gui, const int64_t x, const int64_t y, const int64_t wd, const int64_t ht);

void GUI_FillRect(GUI gui, const int64_t x, const int64_t y, const int64_t wd, const int64_t ht);

void GUI_SetColor(GUI gui, const int8_t r, const int8_t g, const int8_t b, const int8_t alpha);

void GUI_SetColor2(GUI gui, struct Color colour);

void GUI_SetBackgroundColor(GUI gui, struct Color colour);

SDL_Renderer *GUI_GetRenderer(GUI gui);

#endif







