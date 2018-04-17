
#ifndef GRAPH_H
#define GRAPH_H

#include <stdint.h>

enum MazeAlgo { DFS_WITH_BACKTRACKING };

typedef enum MazeAlgo MazeAlgo;

typedef struct graph *GRAPH;

typedef int16_t TILE;

struct Point {
	int16_t x;
	int16_t y;
};

enum Direction {TOP, RIGHT, DOWN, LEFT};

typedef struct Point POINT;
typedef enum Direction DIRECTION;

/* Convenience function. Construct a point from the values. */
POINT make_point(int16_t x, int16_t y);

/* Convenience function. Compute the point obtained after moving one tile in the given direction. */
POINT point_after_moving(POINT p, DIRECTION dir);

/* Check if the specified position (xpos, ypos) is either where the player enters the maze, or where they exit. */
int16_t graph_pos_is_entry_or_exit(GRAPH g, int16_t xpos, int16_t ypos);

/* Allocate memory for a graph of size `width` and initialise according to the specified algorithm.
 * The result should be freed using `graph_free`. */
GRAPH graph_make(int16_t width, MazeAlgo generation_algorithm);

/* Frees a graph created using `graph_make`. */
void graph_free(struct graph *graph);

/* Query some attributes of the graph. */
TILE graph_width(GRAPH g);
POINT graph_start_pos(GRAPH g);
POINT graph_exit_pos(GRAPH g);
TILE graph_has_tile_at(GRAPH g, int16_t xpos, int16_t ypos);

/* Check if the point is inside the graph. */
int in_bounds(GRAPH g, POINT p);

/* Check whether a tile is connected in the given direction. */
int16_t is_connected(TILE nmap, enum Direction dir);

#endif
