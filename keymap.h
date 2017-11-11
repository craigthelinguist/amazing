
#ifndef KEYMAP_H
#define KEYMAP_H

#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

/// This is the range of values for SDLKey.
#define KEY_MAP_SIZE 323

/// The type of the keyboard state, as returned by `SDL_GetKeyboardState`.
typedef const Uint8 *KeyStateMap;

/// These maps record whether a key was first pushed/release in this frame.
extern bool KEY_PRESS_MAP[KEY_MAP_SIZE];
extern bool KEY_RELEASE_MAP[KEY_MAP_SIZE];
extern bool KEY_DOWN_MAP[KEY_MAP_SIZE];

/// Records whether left, up, down, or right was the last direction pressed.
extern SDL_Scancode LAST_DIRECTION_PRESSED;

/// Zero out `KEY_PRESS_MAP` and `KEY_RELEASE_MAP`.
void clear_key_event_maps(void);

#endif