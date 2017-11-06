

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drawing.h"
#include "game_loop.h"
#include "game_state.h"
#include "graph.h"
#include "gui.h"

void draw(GUI gui, int8_t tile, int8_t xpos, int8_t ypos);

TILE make_tile(int top, int right, int down, int left) {
	int8_t tile = 0;
	tile |= (1 << TOP);
	tile |= (1 << RIGHT);
	tile |= (1 << DOWN);
	tile |= (1 << LEFT);
	return tile;
}

int main(int argc, char* argv[]) {
	
	int GRAPH_SIZE = 3;
	if (argc > 1) {
		GRAPH_SIZE = atoi(argv[1]);
	}
	
	SDL_Init(SDL_INIT_VIDEO);
    GUI gui = make_gui(800, 600);
	set_bgcol(gui, (Colour) { 120, 120, 120, 255 });
	GameState game_state = make_game_state();
    init_game(game_state, GRAPH_SIZE, DFS_WITH_BACKTRACKING);
    run_game_loop(gui, game_state);
	
	return 0;
}





