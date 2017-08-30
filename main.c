
#include "game.h"
#include "graph.h"
#include "gui.h"
#include "drawing.h"

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	
	Game game = make_game(gui);
	init_game(game, GRAPH_SIZE, DFS_WITH_BACKTRACKING);
	run_game(game);
	
	return 0;
}





