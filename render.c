
#include <stdio.h>

#include "camera0.h"
#include "drawing.h"
#include "game_state.h"
#include "graphical_constants.h"
#include "render.h"
#include "sprite0.h"
#include "stdbool.h"

// Some default colours.
Colour COLOUR_FLOOR         = { 200, 200, 200, 255 };
Colour COLOUR_WALL          = { 0,   0,   0,   255 };
Colour COLOUR_FLOOR_SPECIAL = { 240, 200, 200, 255 };
Colour COLOUR_BACKGROUND    = { 0,   0,   0,   255 };

// For the walls to look good, their width should be an even number.
const int64_t WALL_WIDTH  = 1;

// Forward declarations.
void draw_tile_walls(GUI gui, GameState game_state, int64_t x, int64_t y);
void draw_maze(GUI gui, GameState game_state);
void draw_entities(GUI gui, GameState game_state);

void render_game(GUI gui, GameState game_state) {
	clear_screen(gui);
	draw_maze(gui, game_state);
	draw_entities(gui, game_state);
	refresh_screen(gui);
}

#define MIN(X,Y) (X < Y ? X : Y)
#define MAX(X,Y) (X > Y ? X : Y)

void draw_entities(GUI gui, GameState game_state) {
    Camera camera = game_state->camera;
    for (int32_t index = 0; index < game_state->num_entities; index++) {
        Entity *entity = &game_state->entities[index];

        // Dimensions of the frame to draw.
		int IMAGE_WD = SPRITE_WD;
		int IMAGE_HT = SPRITE_HT;
        int image_top_y = entity->ypos - MAX(IMAGE_HT - SPRITE_COLLISION_HT, 0);

        // The image to be drawn.
        Image *image = entity->sprite->sprite_sheet;

        // The current frame of the current animation.
        struct Offset offset = get_current_frame(entity->sprite);

        /*
        draw_image_offset(
                gui,
                camera,
                image,
                entity->xpos,
                image_top_y,
                offset.x,
                offset.y,
                IMAGE_WD,
                IMAGE_HT
        );
        */

        set_drawcol(gui, 0, 255, 0, 0);
        draw_rect(gui, camera, entity->xpos, entity->ypos, COLLISION_SIZE, COLLISION_SIZE);
    }
}

void draw_maze(GUI gui, GameState game_state) {

    GRAPH graph = game_state->graph;
    Camera camera = game_state->camera;

	/* Debug info */
	//fprintf(stdout, "Drawing the graph, which has width %d\n", GRAPH_width(graph));
	
	// Draw the background.
	const int8_t WIDTH = GRAPH_width(graph);
	set_drawcol2(gui, COLOUR_FLOOR);
	fill_rect(gui, camera, 0, 0, WIDTH * TILE_WIDTH, WIDTH * TILE_HEIGHT);
	
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
               + TILE_WIDTH * start_tile.x, TILE_HEIGHT * start_tile.y, TILE_WIDTH, TILE_HEIGHT);
	fill_rect(gui, camera,
               + TILE_WIDTH * exit_tile.x,  TILE_HEIGHT * exit_tile.y,  TILE_WIDTH, TILE_HEIGHT);

}

void draw_tile_walls(GUI gui, GameState game_state, int64_t x, int64_t y) {

    GRAPH graph = game_state->graph;
    Camera camera = game_state->camera;

	TILE tile = GRAPH_tileAt(graph, x, y);
	POINT p = make_point(x, y);
	
	// Draw top wall.
	if (!in_bounds(graph, point_after_moving(p, TOP)) || !is_connected(tile, TOP)) {
		fill_rect(gui, camera,
			x * TILE_WIDTH,
			y * TILE_HEIGHT - WALL_WIDTH/2,
			TILE_WIDTH,
			WALL_WIDTH);
	}

	// Draw bottom wall.
	if (!in_bounds(graph, point_after_moving(p, DOWN)) || !is_connected(tile, DOWN)) {
		fill_rect(gui, camera,
			x * TILE_WIDTH,
			y * TILE_HEIGHT + TILE_HEIGHT - WALL_WIDTH/2,
			TILE_WIDTH,
			WALL_WIDTH);
	}

	// Draw right wall.
	if (!in_bounds(graph, point_after_moving(p, RIGHT)) || !is_connected(tile, RIGHT)) {
		fill_rect(gui, camera,
			x * TILE_WIDTH + TILE_WIDTH - WALL_WIDTH/2,
			y * TILE_HEIGHT,
			WALL_WIDTH,
			TILE_HEIGHT);
	}

	// Draw left wall.
	if (!in_bounds(graph, point_after_moving(p, LEFT)) || !is_connected(tile, LEFT)) {
		fill_rect(gui, camera,
			x * TILE_WIDTH - WALL_WIDTH/2,
			y * TILE_HEIGHT,
			WALL_WIDTH,
			TILE_HEIGHT);
	}
	
}



