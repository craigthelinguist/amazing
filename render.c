
#include <stdio.h>

#include "camera0.h"
#include "drawing.h"
#include "game_state.h"
#include "render.h"

// Some default colours.
Colour COLOUR_FLOOR         = { 200, 200, 200, 255 };
Colour COLOUR_WALL          = { 0,   0,   0,   255 };
Colour COLOUR_FLOOR_SPECIAL = { 240, 200, 200, 255 };
Colour COLOUR_BACKGROUND    = { 0,   0,   0,   255 };

// Drawing parameters for tiles in the maze.
const int64_t TILE_WIDTH  = 64;
const int64_t TILE_HEIGHT = 64;

// For the walls to look good, their width should be an even number.
const int64_t WALL_WIDTH  = 1;

const int64_t OFFSET_X    = 10;
const int64_t OFFSET_Y    = 10;

// Forward declarations.
void draw_tile_walls(GUI gui, GameState game_state, int64_t x, int64_t y);
void draw_maze(GUI gui, GameState game_state);

void render_game(GUI gui, GameState game_state) {
	clear_screen(gui);
	draw_maze(gui, game_state);
	refresh_screen(gui);
}

void draw_maze(GUI gui, GameState game_state) {

    GRAPH graph = game_state->graph;
    Camera camera = game_state->camera;

	/* Debug info */
	//fprintf(stdout, "Drawing the graph, which has width %d\n", GRAPH_width(graph));
	
	// Draw the background.
	const int8_t WIDTH = GRAPH_width(graph);
	set_drawcol2(gui, COLOUR_FLOOR);
	fill_rect(gui, camera, OFFSET_X, OFFSET_Y, WIDTH * TILE_WIDTH, WIDTH * TILE_HEIGHT);
	
	/* Debug info 
	POINT start = GRAPH_StartPos(graph);
	POINT exit = GRAPH_ExitPos(graph);
	fprintf(stdout, "Start: (%d, %d) and End: (%d, %d)\n", start.x, start.y, exit.x, exit.y);
	*/
	
	// Draw the start and end tiles.
	set_drawcol2(gui, COLOUR_FLOOR_SPECIAL);
	POINT start_tile = GRAPH_StartPos(graph);
	POINT exit_tile  = GRAPH_ExitPos(graph);
	fill_rect(gui, camera,
              OFFSET_X + TILE_WIDTH * start_tile.x, OFFSET_Y + TILE_HEIGHT * start_tile.y, TILE_WIDTH, TILE_HEIGHT);
	fill_rect(gui, camera,
              OFFSET_X + TILE_WIDTH * exit_tile.x,  OFFSET_Y + TILE_HEIGHT * exit_tile.y,  TILE_WIDTH, TILE_HEIGHT);
	
	// Draw the walls.
	set_drawcol2(gui, COLOUR_WALL);
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < WIDTH; y++) {
			draw_tile_walls(gui, game_state, x, y);
		}
	}
	
}

void draw_tile_walls(GUI gui, GameState game_state, int64_t x, int64_t y) {

    GRAPH graph = game_state->graph;
    Camera camera = game_state->camera;

	TILE tile = GRAPH_tileAt(graph, x, y);
	POINT p = make_point(x, y);
	
	// Draw top wall.
	if (!in_bounds(graph, point_after_moving(p, TOP)) || !is_connected(tile, TOP)) {
		fill_rect(gui, camera,
			OFFSET_X + x * TILE_WIDTH,
			OFFSET_Y + y * TILE_HEIGHT - WALL_WIDTH/2,
			TILE_WIDTH,
			WALL_WIDTH);
	}
	
	// Draw bottom wall.
	if (!in_bounds(graph, point_after_moving(p, DOWN)) || !is_connected(tile, DOWN)) {
		fill_rect(gui, camera,
			OFFSET_X + x * TILE_WIDTH,
			OFFSET_Y + y * TILE_HEIGHT + TILE_HEIGHT - WALL_WIDTH/2,
			TILE_WIDTH,
			WALL_WIDTH);
	}
	
	// Draw right wall.
	if (!in_bounds(graph, point_after_moving(p, RIGHT)) || !is_connected(tile, RIGHT)) {
		fill_rect(gui, camera,
			OFFSET_X + x * TILE_WIDTH + TILE_WIDTH - WALL_WIDTH/2,
			OFFSET_Y + y * TILE_HEIGHT,
			WALL_WIDTH,
			TILE_HEIGHT);
	}
	
	// Draw left wall.
	if (!in_bounds(graph, point_after_moving(p, LEFT)) || !is_connected(tile, LEFT)) {
		fill_rect(gui, camera,
			OFFSET_X + x * TILE_WIDTH - WALL_WIDTH/2,
			OFFSET_Y + y * TILE_HEIGHT,
			WALL_WIDTH,
			TILE_HEIGHT);
	}
	
}



