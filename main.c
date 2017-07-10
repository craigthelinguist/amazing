
#include "game.h"
#include "graph.h"
#include "gui.h"
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
	GUI gui = GUI_Make(800, 600);
	GUI_SetBackgroundColor(gui, (COLOUR) { 120, 120, 120, 255 });
	
	GRAPH graph = GRAPH_Make(GRAPH_SIZE);
	
	GUI_ClearScreen(gui);
	
	draw_graph(graph, gui);
	
	GUI_RefreshScreen(gui);
	
	
	SDL_Delay(5000);
	return 0;
}




