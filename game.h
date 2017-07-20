
#ifndef GAME_H
#define GAME_H

#include "drawing.h"
#include "graph.h"
#include "gui.h"
#include <stdint.h>

void draw_graph(GRAPH graph, GUI gui);

// This is only publically available for debugging purposes. Delete in final version.
void draw_tile(GUI gui, TILE tile, int64_t xpos, int64_t ypos);

#endif
