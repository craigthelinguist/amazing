
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

/// Check if the given `SDL_Scancode` is an arrow key.
bool is_directional_key(SDL_Scancode key) {
    return key == SDL_SCANCODE_DOWN
            || SDL_SCANCODE_UP
            || SDL_SCANCODE_LEFT
            || SDL_SCANCODE_RIGHT;
}

/// Process an event in the game loop as it is polled.
void process_event(SDL_Event e) {

    if (e.type == SDL_QUIT) {
        return;
    }

    // Record the last arrow key pressed so we can update player image appropriately.
    if (e.type == SDL_KEYDOWN) {
        if (is_directional_key(e.key.keysym.scancode)) {
            LAST_DIRECTION_PRESSED = e.key.keysym.scancode;
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
		
	    // Poll events. This implicitly calls SDL_PumpEvents, which will update the state of the keymap under the hood,
        // so we don't have to worry about doing that just yet.
		while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
                goto end_of_game_loop;
            process_event(e);
		}

        // Update the game state.
        while (lag >= MS_PER_UPDATE) {
            Milliseconds update_time = time_ms();
            KeyStateMap key_state = SDL_GetKeyboardState(NULL);
            update_keymaps(key_state);
            update_game(game_state, key_state, update_time);
            lag -= MS_PER_UPDATE;
        }

		render_game(gui, game_state);
		
	}

    end_of_game_loop:;


	/* Main game loop goes here. */
	// SDL_Delay(5000);
	
}



