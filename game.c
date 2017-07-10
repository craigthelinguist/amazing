
#include "game.h"
#include <stdio.h>

// Some default colours.
struct Color COLOR_FLOOR         = { 200, 200, 200, 255 };
struct Color COLOR_WALL          = { 0,   0,   0,   255 };
struct Color COLOR_FLOOR_SPECIAL = { 240, 200, 200, 255 };
struct Color COLOR_BACKGROUND	 = { 0,   0,   0,   255 };

// Drawing parameters for tiles in the maze.
const int64_t TILE_WIDTH  = 32;
const int64_t TILE_HEIGHT = 32;

// For the walls to look good, their width should be an even number.
const int64_t WALL_WIDTH  = 2;

const int64_t OFFSET_X    = 10;
const int64_t OFFSET_Y    = 10;

void draw_tile_walls(GUI gui, GRAPH graph, int64_t x, int64_t y);

void draw_graph(GRAPH graph, GUI gui) {
	fprintf(stdout, "Drawing the graph, which has width %d\n", GRAPH_width(graph));
	
	// Draw the background.
	const int8_t WIDTH = GRAPH_width(graph);
	GUI_SetColor2(gui, COLOR_FLOOR);
	GUI_FillRect(gui, OFFSET_X, OFFSET_Y, WIDTH * TILE_WIDTH, WIDTH * TILE_HEIGHT);
	
	// Draw the start and end tiles.
	POINT start = GRAPH_StartPos(graph);
	POINT exit = GRAPH_ExitPos(graph);
	fprintf(stdout, "Start: (%d, %d) and End: (%d, %d)\n", start.x, start.y, exit.x, exit.y);
	GUI_SetColor2(gui, COLOR_FLOOR_SPECIAL);
	POINT start_tile = GRAPH_StartPos(graph);
	POINT exit_tile  = GRAPH_ExitPos(graph);
	GUI_FillRect(gui, OFFSET_X + TILE_WIDTH * start_tile.x, OFFSET_Y + TILE_HEIGHT * start_tile.y, TILE_WIDTH, TILE_HEIGHT);
	GUI_FillRect(gui, OFFSET_X + TILE_WIDTH * exit_tile.x,  OFFSET_Y + TILE_HEIGHT * exit_tile.y,  TILE_WIDTH, TILE_HEIGHT);
	
	// Draw the walls.
	GUI_SetColor2(gui, COLOR_WALL);
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < WIDTH; y++) {
			draw_tile_walls(gui, graph, x, y);
	}
	}
	
}

void draw_tile_walls(GUI gui, GRAPH graph, int64_t x, int64_t y) {
	TILE tile = GRAPH_tileAt(graph, x, y);
	POINT p = make_point(x, y);
	
	// Draw top wall.
	if (!in_bounds(graph, point_after_moving(p, TOP)) || !is_connected(tile, TOP)) {
		GUI_FillRect(gui,
			OFFSET_X + x * TILE_WIDTH,
			OFFSET_Y + y * TILE_HEIGHT - WALL_WIDTH/2,
			TILE_WIDTH,
			WALL_WIDTH);
	}
	
	// Draw bottom wall.
	if (!in_bounds(graph, point_after_moving(p, DOWN)) || !is_connected(tile, DOWN)) {
		GUI_FillRect(gui,
			OFFSET_X + x * TILE_WIDTH,
			OFFSET_Y + y * TILE_HEIGHT + TILE_HEIGHT - WALL_WIDTH/2,
			TILE_WIDTH,
			WALL_WIDTH);
	}
	
	// Draw right wall.
	if (!in_bounds(graph, point_after_moving(p, RIGHT)) || !is_connected(tile, RIGHT)) {
		GUI_FillRect(gui,
			OFFSET_X + x * TILE_WIDTH + TILE_WIDTH - WALL_WIDTH/2,
			OFFSET_Y + y * TILE_HEIGHT,
			WALL_WIDTH,
			TILE_HEIGHT);
	}
	
	// Draw left wall.
	if (!in_bounds(graph, point_after_moving(p, LEFT)) || !is_connected(tile, LEFT)) {
		GUI_FillRect(gui,
			OFFSET_X + x * TILE_WIDTH - WALL_WIDTH/2,
			OFFSET_Y + y * TILE_HEIGHT,
			WALL_WIDTH,
			TILE_HEIGHT);
	}
	
}

