
#ifndef GAME_H
#define GAME_H

#include "drawing.h"
#include "graph.h"
#include "gui.h"
#include "maze_algos.h"
#include <stdint.h>

typedef struct Game *Game;

Game make_game(GUI gui);

void free_game(Game game);

void init_game(Game game, int16_t maze_size, Algorithm generation_algorithm);

void run_game(Game game);

void draw_graph(GRAPH graph, GUI gui);

#endif
