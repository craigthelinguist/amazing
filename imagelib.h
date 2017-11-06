
#ifndef IMAGELIB_H
#define IMAGELIB_H

#include "image.h"
#include <SDL2/SDL.h>

enum ImageLibErr {
    OK,
    DIRECTORY_NOT_FOUND,    // if the base directory could not be found
    FILE_NOT_FOUND,         // if the specified file could not be found
    LIBRARY_FULL,           // if the library has no space to load any more images
    FILE_NAME_TOO_LONG,     // if trying to load an image with a name too long
    SDL_ERROR,              // some unspecified SDL error
    NO_SUCH_IMAGE           // if the library has no image with the specified name
};

typedef enum ImageLibErr ImageLibErr;

/// This is set whenever a library call fails.
extern ImageLibErr IMAGE_LIB_ERR;

/// Initialise the image library. Afterwards, you can load and retrieve up to `capacity` images from the specified
/// directory. If you pass `NULL`, `imagelib` will look in the current directory for images. If the library fails to
/// initialise, then `IMAGE_LIB_ERR` will be set to one of the following error values: DIRECTORY_NOT_FOUND.
bool imagelib_init(char *asset_directory, const int capacity);

/// Frees all images that have been loaded into the library, and the library itself.
void imagelib_free();

/// Get the image in the library with the given filename. If the image does not exist, `NULL` will be returned, and
/// `IMAGE_LIB_ERR` will be set to one of the following error values: NO_SUCH_IMAGE.
struct Image *imagelib_get(char *fname);

/// Load the image at the specified file name into the given renderer. If the image does not exist, `false` will be
/// returned, and `IMAGE_LIB_ERR` will be set to one of the following error values: DIRECTORY_NOT_FOUND, FILE_NOT_FOUND,
/// FILE_NAME_TOO_LONG, or LIBRARY_FULL.
bool imagelib_load(char *fname, SDL_Renderer *renderer);

#endif
