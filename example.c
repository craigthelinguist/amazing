
#include "image.h"
#include "imagelib0.h"
#include "imagelib.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include <SDL2/SDL.h>

#include <SDL_image.h>


struct colour {
    int red;
    int green;
    int blue;
    int alpha;
};

Uint32 pixel_at(SDL_Surface *surface, SDL_Renderer *renderer, int x, int y) {

    if (x < 0 || y < 0 || x >= surface->w || y >= surface->h) {
        fprintf(stderr, "Accessing pixel that is out of bounds.");
        exit(12048);
    }

    if (SDL_MUSTLOCK(surface))
        SDL_LockSurface(surface);

    Uint32 *pixels = (Uint32 *)surface->pixels;

    Uint8 r, g, b;

    SDL_GetRGB(pixels[100], surface->format, &r, &g, &b);

    if (SDL_MUSTLOCK(surface))
        SDL_UnlockSurface(surface);

}

int main(int argc, char *argv[]) {

    // Initialise SDL.

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Could not initialise SDL. ERror: %s\n", SDL_GetError());
        exit(23408);
    }

    SDL_Window *window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 680, 400, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Could not initialise SDL window. Error: %s\n", SDL_GetError());
        exit(2309);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Could not initialise SDL_Renderer. Error: %s\n", SDL_GetError());
        exit(34909);
    }

    // Initialise the image library.

    const char *ASSET_DIRECTORY = "assets";
    const char *FNAME = "celes-down";

    SDL_Surface *surface = IMG_Load("assets/maze-grid-pathing.png");
    if (surface == NULL) {
        IMAGELIB_ERRCODE = SDL_ERROR;
        return false;
    }

    pixel_at(surface, renderer, 0, 0);

    SDL_Delay(1000);

    printf("done\n");
}
