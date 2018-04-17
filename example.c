
#include "image.h"
#include "imagelib0.h"
#include "imagelib.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include <SDL2/SDL.h>

struct colour {
    int red;
    int green;
    int blue;
    int alpha;
};

void pixel_at(struct image *img, SDL_Renderer *renderer, uint16_t x, uint16_t y) {

    if (x > img->wd || y > img->ht) {
        fprintf(stderr, "Accessing pixel not on the image.");
        exit(12409);
    }

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

    if (!imagelib_init(ASSET_DIRECTORY, 10)) {
        fprintf(stderr, "Failed to load image library.");
        print_imagelib_errmsg();
        exit(12349084);
    }

    if (!imagelib_load(FNAME, renderer)) {
        fprintf(stderr, "Failed to load image %s\n", FNAME);
        print_imagelib_errmsg();
        exit(2340923409);
    }

    struct image *image = imagelib_get(FNAME);
    if (!image) {
        fprintf(stderr, "ERror, image is null after loading.\n");
        exit(23409234);
    }

    pixel_at(image, renderer, 0, 0);

    SDL_Delay(1000);

    printf("done\n");
}
