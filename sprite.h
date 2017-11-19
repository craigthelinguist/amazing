
#ifndef SPRITE_H
#define SPRITE_H

#include <stdbool.h>

typedef struct Animation *Animation;

typedef struct Sprite *Sprite;

/// Set the current animation of the sprite.
/// \param sprite : the sprite to update.
/// \param name : the name of the animation.
/// \param time_ms : the time of the game-state update.
/// \return true iff there is an animation with that name.
bool set_anim_name(Sprite sprite, const char *name, long long time_ms);

#endif
