
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>

#include "game_loop.h"
#include "keymap.h"
#include "render.h"

typedef unsigned long long Milliseconds;

/// Get the current time with millisecond precision. This is platform-specific.
long time_ms(void) {
    struct timeval time_val;
    gettimeofday(&time_val, NULL);
    unsigned long long ms_since_epoch =
            (Milliseconds)(time_val.tv_sec) * 1000 +
            (Milliseconds)(time_val.tv_usec) / 1000;
    return ms_since_epoch;
}

/// Updates the key maps based on the current state of the keyboard.
void update_keymaps(KeyStateMap key_state) {
    /* SDL_PumpEvents(); // unnecessary? PollEvent calls this, apparently. */
    for (SDL_Scancode key = 0; key < KEY_MAP_SIZE; key++) {
        if (key_state[key] && !KEY_DOWN_MAP[key]) {
            KEY_PRESS_MAP[key] = KEY_DOWN_MAP[key] = true;
        } else if (key_state[key] && KEY_PRESS_MAP[key]) {
            KEY_PRESS_MAP[key] = false;
        } else if (!key_state[key] && KEY_DOWN_MAP[key]) {
            KEY_DOWN_MAP[key] = false;
            KEY_RELEASE_MAP[key] = true;
        } else if (!key_state[key] && KEY_RELEASE_MAP[key]) {
            KEY_RELEASE_MAP[key] = false;
        }
    }
}

/// Run the main game loop.
void run_game_loop(GUI gui, GameState game_state) {

	SDL_Event e;
	const Milliseconds MS_PER_UPDATE = (long long) (1000.0 / 60.0); // 50 FPS
	Milliseconds last_update = time_ms();
	Milliseconds lag = 0;
	
	while(true) {
		
	    // Scale game events to elapsed time.
		Milliseconds now = time_ms();
		Milliseconds elapsed = now - last_update;
		last_update = now;
        lag += elapsed;
		
	    // Poll events. This implicitly calls SDL_PumpEvents, which will update
    	// the state of the keymap under the hood. Unless it's a quit event, we
    	// don't actually do anything yet until it's time to update the game state.
		while(SDL_PollEvent(&e) != 0) {
            // printf("Key %s\n", SDL_GetKeyName(e.key.keysym.sym));
			if (e.type == SDL_QUIT) return;
		}

        // Update the game state.
        while (lag >= MS_PER_UPDATE) {
            KeyStateMap key_state = SDL_GetKeyboardState(NULL);
            update_keymaps(key_state);
            update_game(game_state, key_state);
            lag -= MS_PER_UPDATE;
        }

		render_game(gui, game_state);
		
	}
	
	/* Main game loop goes here. */
	// SDL_Delay(5000);
	
}



