
#ifndef IMAGELIB0_H
#define IMAGELIB0_H

#ifdef __unix__
	#include <cjson/cJSON.h>
	#include <SDL2/SDL.h>
#elif defined(_WIN32) || defined(WIN32)
	#include <cjson\cJSON.h>
	#include <SDL2\SDL.h>
#endif

#include "image.h"
#include "imagelib.h"
#include "sprite0.h"

/// Validate whether the given JSON object describes a sprite for the given sprite sheet. If the JSON object does not
/// correctly describe a sprite, then `false` is returned, and IMAGE_LIB_ERR is set.
bool json_is_sprite(cJSON *root, Image *sprite_sheet);

/// Create a new sprite from the given JSON object. No error-checking is done if the JSON object is invalid; you should
/// call `json_is_sprite` to ensure that it is valid before calling this. A fresh Sprite is allocated, which the caller
/// should free.
Sprite sprite_from_json(cJSON *obj, Image *sprite_sheet);

/// Resolve the filename, turning it into an absolute filepath. This mallocs a string, which the caller must free. If
/// the file does not exist, then `NULL` is returned, and `IMAGE_LIB_ERR` is set to one of the following:
/// FILE_NAME_TOO_LONG, FILE_NOT_FOUND.
char *resolve_fname(const char *fname);

/// Construct a new string by appending `suffix` onto `string`. This allocates memory, which should be freed by the
/// caller.
char *fname_append(const char *string, const char *suffix);

/// Strip the asset directory from the given `fpath`, returning the `fname`. This allocates memory which the user must
/// free.
char *strip_directory(const char *fpath);

/// Load a cJSON object from the given file.
cJSON *load_json(const char *fname);
cJSON *load_json_absolute(const char *fpath);

#endif
