
#include "keymap.h"

// TODO: we can't do the simple thing and index by SDL_Keycode because the values are weird (they're not all
// TODO: after another). Might have to do a manual shift to get them to fit into a short array.
bool KEY_PRESS_MAP[KEY_MAP_SIZE];
bool KEY_RELEASE_MAP[KEY_MAP_SIZE];

void clear_key_event_maps(void) {
    for (SDL_Keycode key = 0; key < KEY_MAP_SIZE; key++) {
        KEY_PRESS_MAP[key] = 0;
        KEY_RELEASE_MAP[key] = 0;
    }
}

