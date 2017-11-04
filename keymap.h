
#ifndef KEYMAP_H
#define KEYMAP_H

#include <SDL2/SDL.h>
#include <stdbool.h>

/// This is the range of values for SDLKey.
#define KEY_MAP_SIZE 323

/// These maps record whether a key was first pushed/release in this frame.
extern bool KEY_PRESS_MAP[KEY_MAP_SIZE];
extern bool KEY_RELEASE_MAP[KEY_MAP_SIZE];

/// Zero out `KEY_PRESS_MAP` and `KEY_RELEASE_MAP`.
void clear_key_event_maps(void);

/// Check if `key` was pressed this frame.
bool was_key_pressed(SDL_Keycode key);

/// Check if `key` was released this frame.
bool was_key_released(SDL_Keycode key);

#endif