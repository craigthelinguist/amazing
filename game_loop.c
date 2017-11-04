
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "game_loop.h"
#include "keymap.h"
#include "render.h"

/// Updates the key maps based on the current state of the keyboard.
void update_keymaps(void) {
  const Uint8 *key_state = SDL_GetKeyboardState(NULL);
  for (SDL_Keycode key = 0; key < KEY_MAP_SIZE; key++) {
    KEY_PRESS_MAP[key] ^= key_state[key];
    KEY_RELEASE_MAP[key] ^= key_state[key];
  }  
}

/// Run the main game loop.
void run_game_loop(GUI gui, GameState game_state) {

	SDL_Event e;
	const time_t MS_PER_UPDATE = 400;
	time_t last_update = 0;
	time_t lag = 0;
	
	while(true) {
		
	    // Scale game events to elapsed time.
		time_t now = time(&now);
		time_t elapsed = now - last_update;
		last_update = now;
		
	    // Poll events. This implicitly calls SDL_PumpEvents, which will update
    	// the state of the keymap under the hood. Unless it's a quit event, we
    	// don't actually do anything yet until it's time to update the game state.
		while(!SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) return;
		}
		
    	// Update the game state.
		while (lag >= MS_PER_UPDATE) {
      		update_keymaps();
			update_game(game_state);
			lag -= MS_PER_UPDATE;
		}
		
		render_game(gui, game_state);
		
	}
	
	/* Main game loop goes here. */
	// SDL_Delay(5000);
	
}



