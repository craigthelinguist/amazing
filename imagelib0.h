
#ifndef IMAGELIB0_H
#define IMAGELIB0_H

#include <cjson\cJSON.h>
#include <SDL2\SDL.h>

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

#endif
