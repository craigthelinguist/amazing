

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drawing.h"
#include "game_loop.h"
#include "game_state.h"
#include "graph.h"
#include "gui0.h"
#include "image.h"
#include "imagelib.h"

static char *ASSET_DIRECTORY = "assets";

int main(int argc, char* argv[]) {

	// Parse command-line arguments.
	int GRAPH_SIZE = 3;
	if (argc > 1) {
		GRAPH_SIZE = atoi(argv[1]);
	}

	// Set up SDL and the gui.
	SDL_Init(SDL_INIT_VIDEO);
    GUI gui = make_gui(800, 600);
	set_bgcol(gui, (Colour) { 120, 120, 120, 255 });

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
	return 0;
}





