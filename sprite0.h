
#ifndef SPRITE0_H
#define SPRITE0_H

#include <SDL2/SDL.h>

#include <stdint.h>

#include "sprite.h"
#include "image.h"

#define MAX_ANIMATION_NAME_LEN 64
#define SPRITE_WD = 32;
#define SPRITE_HT = 48;

/// An offset is a single frame in an animation. (left, top) is the coordinates of the top-left of a single frame in the
/// sprite sheet.
struct Offset {
    uint16_t left;
    uint16_t top;
};

/// An animation is a fixed collection of frames. Each animation has a `name`, by which they are referred, and a
/// `frame_delay`, which controls how quickly an animation toggles through its frames.
struct Animation {
    char name[MAX_ANIMATION_NAME_LEN];
    long long frame_delay;
    int8_t num_frames;
    struct Offset offsets[];
};

/// A sprite is a fixed collection of animations stored on a sprite sheet. At any given time, a sprite is only
/// performing one particular animation, which is `current_animation`.
struct Sprite {
    Image *sprite_sheet;
    int8_t num_animations;
    struct Animation *current_animation;
    int8_t current_frame;
    long long frame_age;
    struct Animation animations[];
};

/// Get the animation stored at the specified index. A runtime error occurs if the index is out of bounds.
Animation get_anim_by_index(Sprite sprite, int16_t index);

/// Get the animation with the given name. `NULL` is returned if no such animation exists.
Animation get_anim_by_name(Sprite sprite, const char *anim_name);

#endif
