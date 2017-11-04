
#include "keymap.h"

bool KEY_PRESS_MAP[KEY_MAP_SIZE];
bool KEY_RELEASE_MAP[KEY_MAP_SIZE];

void clear_key_event_maps(void) {
    for (SDL_Keycode key = 0; key < KEY_MAP_SIZE; key++) {
        KEY_PRESS_MAP[key] = 0;
        KEY_RELEASE_MAP[key] = 0;
    }
}

bool was_key_pressed(SDL_Keycode key) {
  return KEY_PRESS_MAP[key];
}

bool was_key_released(SDL_Keycode key) {
  return KEY_RELEASE_MAP[key];
}
