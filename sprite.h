
#ifndef SPRITE_H
#define SPRITE_H

#include <stdbool.h>

typedef struct Animation *Animation;

typedef struct Sprite *Sprite;

/// An offset is a single frame in an animation. (left, top) is the coordinates of the top-left of a single frame in the
/// sprite sheet.
struct Offset {
    uint16_t x;
    uint16_t y;
};

/// Set the current animation of the sprite.
/// \param sprite : the sprite to update.
/// \param name : the name of the animation.
/// \param time_ms : the time of the game-state update.
/// \return true iff there is an animation with that name.
bool set_anim_name(Sprite sprite, const char *name, long long time_ms);

/// Updates the frame for the current animation (if it needs to be updated).
void animate(Sprite sprite, long long time_ms);

/// Get the offset into the sprite's sprite sheet of its current frame.
struct Offset get_current_frame(Sprite sprite);

#endif
