

#ifndef GRAPH0_H
#define GRAPH0_H

#include "graph.h"

typedef struct graph graph;

struct graph {

    // Can be at most 256. A maze is always a square.
    int16_t width;

    // Start of the maze.
    POINT start_pos;

    // You win if you get here.
    POINT exit_pos;

    // 2d array of neighbours. nmap[i] is a bitpacked list of neighbours for tile i. */
    TILE nmap[];

};

void connect_edge(graph *g, POINT p, enum Direction dir);

#endif


