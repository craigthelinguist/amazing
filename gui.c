
#include "colours.h"
#include "gui0.h"
#include "point.h"
#include <stdlib.h>

GUI make_gui(const uint16_t WIDTH, const uint16_t HEIGHT) {
    SDL_Window *window = SDL_CreateWindow("Amazing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL)
        return NULL;
    struct Gui *gui = malloc(sizeof(struct Gui));
    gui->window = window;
    gui->screen_wd = WIDTH;
    gui->screen_ht = HEIGHT;
    gui->renderer = SDL_CreateRenderer(window, -1, 0);

    // Enables you to draw things with transparency.
    SDL_SetRenderDrawBlendMode(gui->renderer, SDL_BLENDMODE_BLEND);

    // Default background colour is white.
    gui->bgcol = make_colour(255, 255, 255, 255);

    return gui;
}

Point_Int32 offset_point(GUI gui, Camera camera, uint32_t x, uint32_t y) {

    // Figure out the (x,y) of the top-left of the camera.
    Point_Int32 camera_topleft = (Point_Int32) {
            camera->centre_x - gui->screen_wd / 2,
            camera->centre_y - gui->screen_ht / 2
    };

    // Offset (x,y) by the camera, so it is relative to (0,0).
    return (Point_Int32) {
            x - camera_topleft.x,
            y - camera_topleft.y
    };

}

const SDL_PixelFormat *get_screen_format(GUI gui) {
    return SDL_GetWindowSurface(gui->window)->format;
}

void free_gui(GUI gui) {
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

void set_bgcol(GUI gui, Colour colour) {
    gui->bgcol = colour;
}

Colour get_bgcol(GUI gui) {
    return gui->bgcol;
}

SDL_Renderer *get_renderer(GUI gui) {
    return gui->renderer;
}

uint16_t screen_wd(GUI gui) { return gui->screen_wd; }

uint16_t screen_ht(GUI gui) { return gui->screen_ht; }




