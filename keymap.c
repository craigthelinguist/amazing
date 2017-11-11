
#include "keymap.h"

bool KEY_PRESS_MAP[KEY_MAP_SIZE];
bool KEY_RELEASE_MAP[KEY_MAP_SIZE];
bool KEY_DOWN_MAP[KEY_MAP_SIZE];

SDL_Scancode LAST_DIRECTION_PRESSED = SDLK_DOWN;


void clear_key_event_maps(void) {
    for (SDL_Scancode key = 0; key < KEY_MAP_SIZE; key++) {
        KEY_PRESS_MAP[key] = false;
        KEY_RELEASE_MAP[key] = false;
        KEY_DOWN_MAP[key] = false;
    }
}

