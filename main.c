

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

	// Load the image.
	if (!imagelib_load("celes.png", gui->renderer)) {
        switch (IMAGE_LIB_ERR) {
            case DIRECTORY_NOT_FOUND:   fprintf(stderr, "Could not find directory %s", ASSET_DIRECTORY); break;
            case FILE_NOT_FOUND:        fprintf(stderr, "Could not find asset `celes.png`"); break;
            case FILE_NAME_TOO_LONG:    fprintf(stderr, "`celes.png` is too long a file name."); break;
            case LIBRARY_FULL:          fprintf(stderr, "The image library is full."); break;
            default:                    fprintf(stderr, "Unknown error %d", IMAGE_LIB_ERR); break;
        }
        exit(20);
	}

	// Set up the game state.
	GameState game_state = make_game_state();
    init_game(game_state, GRAPH_SIZE, DFS_WITH_BACKTRACKING);

	// Run the game.
	run_game_loop(gui, game_state);
	return 0;
}





