
#ifndef GRAPH_H
#define GRAPH_H

#include "maze_algos.h"
#include <stdint.h>

typedef struct Graph *GRAPH;
typedef int16_t TILE;

struct Point {
	int16_t x;
	int16_t y;
};

enum Direction {TOP, RIGHT, DOWN, LEFT};

typedef struct Point POINT;
typedef enum Direction DIRECTION;

POINT make_point(int16_t x, int16_t y);
int in_bounds(GRAPH g, POINT p);
POINT point_after_moving(POINT p, DIRECTION dir);

TILE   GRAPH_width(GRAPH g);
TILE   GRAPH_tileAt(GRAPH g, int16_t xpos, int16_t ypos);
int16_t GRAPH_isEntryOrExit(GRAPH g, int16_t xpos, int16_t ypos);
GRAPH  GRAPH_Make(int16_t width, Algorithm generation_algorithm);
void GRAPH_Free(struct Graph *graph);
POINT  GRAPH_StartPos(GRAPH g);
POINT  GRAPH_ExitPos(GRAPH g);

int16_t is_connected(TILE nmap, enum Direction dir);





#endif
