

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drawing.h"
#include "game_loop.h"
#include "game_state.h"
#include "graph0.h"
#include "gui0.h"
#include "image.h"
#include "imagelib.h"

static char *ASSET_DIRECTORY = "assets";

int main(int argc, char* argv[]) {

	// Parse command-line arguments.
	int16_t GRAPH_SIZE = 3;
	if (argc > 1) {
		GRAPH_SIZE = (int16_t) atoi(argv[1]);
	}

	// Set up SDL and the gui.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Could not initialise SDL. Error: %s\n", SDL_GetError());
    } else {
        fprintf(stderr, "Successfully initialised SDL.\n");
    }

    GUI gui = make_gui(800, 600);
	set_bgcol(gui, (Colour) { 15, 15, 15, 255 });

	// Set up the image library.
	if (!imagelib_init(ASSET_DIRECTORY, 5)) {
		fprintf(stderr, "Could not find directory '%s' while initialising library", ASSET_DIRECTORY);
        exit(30);
	}

	// Set up the game state.
	GameState game_state = make_game_state(gui);
    init_game(game_state, gui, GRAPH_SIZE, DFS_WITH_BACKTRACKING);

	// Run the game.
	run_game_loop(gui, game_state);

    fprintf(stderr, "Finished.\n");
	return 0;
}
