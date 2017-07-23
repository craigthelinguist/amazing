
#include "event_q.h"
#include "game.h"
#include "graph.h"
#include <stdio.h>

// Some default colours.
Colour COLOUR_FLOOR         = { 200, 200, 200, 255 };
Colour COLOUR_WALL          = { 0,   0,   0,   255 };
Colour COLOUR_FLOOR_SPECIAL = { 240, 200, 200, 255 };
Colour COLOUR_BACKGROUND	 = { 0,   0,   0,   255 };

// Drawing parameters for tiles in the maze.
const int64_t TILE_WIDTH  = 32;
const int64_t TILE_HEIGHT = 32;

// For the walls to look good, their width should be an even number.
const int64_t WALL_WIDTH  = 2;

const int64_t OFFSET_X    = 10;
const int64_t OFFSET_Y    = 10;



void draw_tile_walls(GUI gui, GRAPH graph, int64_t x, int64_t y);










struct Game {
	GUI gui;
	Event_Q event_queue;
	GRAPH game_world;
	char is_initialised;
};





/**
	Allocate memory for a new game. You must pass in the GUI which it should
	draw to, and then call init_game with the kind of maze you want it to
	generate.
**/
Game make_game(GUI gui) {	
	struct Game *game = malloc(sizeof(struct Game));
	Event_Q event_queue = make_eventq();
	game->gui = gui;
	game->event_queue = event_queue;
	game->game_world = NULL;
	game->is_initialised = 0;
	return game;
}

/**
	Frees the memory used by this game. The GUI that this game uses will not
	be freed; the caller must manually free that.
**/
void free_game(Game game) {
	if (game->is_initialised) {
		GRAPH_Free(game->game_world);
	}
	game->game_world = NULL;
	game->is_initialised = 0;
	free_eventq(game->event_queue);
	game->event_queue = NULL;
	game->gui = NULL;
}

/**
	Initialise the game world using the parameters given. Memory will be
	allocated for the game world. The memory for the game world is freed
	when the memory for the game is freed.
**/
void init_game(Game game, int16_t maze_size, Algorithm generation_algorithm) {
	game->game_world = GRAPH_Make(maze_size, generation_algorithm);
	game->is_initialised = 1;
}

void run_game(Game game) {

	if (!game->is_initialised) {
		fprintf(stderr, "Cannot run the game; it has not been initialised properly.");
		return;
	}
	
	/* Main game loop goes here. */
	clear_screen(game->gui);
	draw_graph(game->game_world, game->gui);
	refresh_screen(game->gui);
	SDL_Delay(5000);
	
}





void draw_graph(GRAPH graph, GUI gui) {
	fprintf(stdout, "Drawing the graph, which has width %d\n", GRAPH_width(graph));
	
	// Draw the background.
	const int8_t WIDTH = GRAPH_width(graph);
	set_drawcol2(gui, COLOUR_FLOOR);
	fill_rect(gui, OFFSET_X, OFFSET_Y, WIDTH * TILE_WIDTH, WIDTH * TILE_HEIGHT);
	
	// Draw the start and end tiles.
	POINT start = GRAPH_StartPos(graph);
	POINT exit = GRAPH_ExitPos(graph);
	fprintf(stdout, "Start: (%d, %d) and End: (%d, %d)\n", start.x, start.y, exit.x, exit.y);
	set_drawcol2(gui, COLOUR_FLOOR_SPECIAL);
	POINT start_tile = GRAPH_StartPos(graph);
	POINT exit_tile  = GRAPH_ExitPos(graph);
	fill_rect(gui, OFFSET_X + TILE_WIDTH * start_tile.x, OFFSET_Y + TILE_HEIGHT * start_tile.y, TILE_WIDTH, TILE_HEIGHT);
	fill_rect(gui, OFFSET_X + TILE_WIDTH * exit_tile.x,  OFFSET_Y + TILE_HEIGHT * exit_tile.y,  TILE_WIDTH, TILE_HEIGHT);
	
	// Draw the walls.
	set_drawcol2(gui, COLOUR_WALL);
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
		fill_rect(gui,
			OFFSET_X + x * TILE_WIDTH,
			OFFSET_Y + y * TILE_HEIGHT - WALL_WIDTH/2,
			TILE_WIDTH,
			WALL_WIDTH);
	}
	
	// Draw bottom wall.
	if (!in_bounds(graph, point_after_moving(p, DOWN)) || !is_connected(tile, DOWN)) {
		fill_rect(gui,
			OFFSET_X + x * TILE_WIDTH,
			OFFSET_Y + y * TILE_HEIGHT + TILE_HEIGHT - WALL_WIDTH/2,
			TILE_WIDTH,
			WALL_WIDTH);
	}
	
	// Draw right wall.
	if (!in_bounds(graph, point_after_moving(p, RIGHT)) || !is_connected(tile, RIGHT)) {
		fill_rect(gui,
			OFFSET_X + x * TILE_WIDTH + TILE_WIDTH - WALL_WIDTH/2,
			OFFSET_Y + y * TILE_HEIGHT,
			WALL_WIDTH,
			TILE_HEIGHT);
	}
	
	// Draw left wall.
	if (!in_bounds(graph, point_after_moving(p, LEFT)) || !is_connected(tile, LEFT)) {
		fill_rect(gui,
			OFFSET_X + x * TILE_WIDTH - WALL_WIDTH/2,
			OFFSET_Y + y * TILE_HEIGHT,
			WALL_WIDTH,
			TILE_HEIGHT);
	}
	
}

