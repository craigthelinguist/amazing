
#include "graph0.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "utils0.h"

void terminate(const char *err_msg) {
	fprintf(stderr, err_msg);
	exit(1);
}

int16_t graph_width(graph *g) { return g->width; }

int16_t graph_has_tile_at(graph *g, int16_t xpos, int16_t ypos) {
	return g->nmap[xpos * graph_width(g) + ypos];
}

POINT graph_start_pos(graph *g) { return g->start_pos; }
POINT graph_exit_pos (graph *g) { return g->exit_pos;  }

int16_t graph_pos_is_entry_or_exit(graph *g, int16_t xpos, int16_t ypos) {
	return (g->start_pos.x = xpos && g->start_pos.y == ypos)
		|| (g->exit_pos.x = xpos && g->exit_pos.y == ypos);
}

POINT make_point(int16_t x, int16_t y) {
	return (POINT) {x, y};
}

enum Direction opposite_direction(enum Direction dir) {
	switch(dir) {
		case TOP:   return DOWN;
		case DOWN:  return TOP;
		case LEFT:  return RIGHT;
		case RIGHT: return LEFT;
		default:
			fprintf(stderr, "Error: finding opposite direction of unknown enum %d\n", dir);
			exit(1);
	}
}

void generate_maze(graph *g);

graph *graph_make(int16_t width, MazeAlgo generation_algorithm) {
	graph *g = malloc(sizeof(graph) + sizeof(int16_t) * width * width);
	g->width = width;
	
	// Initialise the neighbour map.
	for (int i = 0; i < width * width; i++) {
		g->nmap[i] = 0;
	}
	
	switch(generation_algorithm) {
		case DFS_WITH_BACKTRACKING:
			generate_maze(g);
			break;
			
		default:
			free(g);
			g = NULL;
			break;
	}
	
	return g;
}

void graph_free(graph *graph) {
	free(graph);

}

POINT point_after_moving(POINT p, enum Direction dir) {
	POINT p2 = make_point(p.x, p.y);
	switch (dir) {
		case TOP:
			p2.y -= 1;
			break;
		case LEFT:
			p2.x -= 1;
			break;
		case DOWN:
			p2.y += 1;
			break;
		case RIGHT:
			p2.x += 1;
			break;
	}
	return p2;
}

int in_bounds(GRAPH g, POINT p) {
	return p.x >= 0 && p.y >= 0 && p.x < g->width && p.y < g->width;
}

void check_point_in_bounds(graph *g, POINT p) {
	if (!in_bounds(g, p)) {
		fprintf(stderr, "SNAP: Point (%d, %d) is out of bounds.\n", p.x, p.y);
		exit(1);
	}
}

/**
 * Connects an edge in a graph. This means we update two tiles so they are mutually connected.
 * NB: don't rename this to `connect`, as it seems to clash with something in the latest SDL2 libraries.
 */
void connect_edge(graph *g, POINT p, enum Direction dir) {
	check_point_in_bounds(g, p);
	g->nmap[p.y * g->width + p.x] |= (1 << dir);
	p = point_after_moving(p, dir);
	check_point_in_bounds(g, p);
	g->nmap[p.y * g->width + p.x] |= (1 << opposite_direction(dir));
}

int16_t is_connected(TILE nmap, enum Direction dir) {
	return !!(nmap & (1 << dir));
}

int16_t is_connected2(graph *g, POINT p, enum Direction dir) {
	TILE t = graph_has_tile_at(g, p.x, p.y);
	return !!(t & (1 << dir));
}

int has_been_visited(graph *g, char *visited, POINT p) {
	return visited[g->width * p.x + p.y];
}

int all_neighbours_visited(graph *g, char *visited, POINT p) {
	for (DIRECTION dir = 0; dir < 4; dir++) {
		POINT p2 = point_after_moving(p, dir);
		if (in_bounds(g, p2) && !has_been_visited(g, visited, p2))
			return 0;
	}
	return 1;
}

void mark_as_visited(graph *g, char *visited, POINT p) {
	visited[g->width * p.x + p.y] = 1;
}

DIRECTION random_unvisited_neighbour(GRAPH g, char *visited, POINT p) {
	int total_iters = 0;
	DIRECTION dir = rand() % 4;
	POINT p2 = point_after_moving(p, dir);
	
	while (!in_bounds(g, p2) || has_been_visited(g, visited, p2)) {
		if (total_iters == 4)
			terminate("ERROR: Finding unvisited neighbour of a tile whose every neighbour has been visited.\n");
		total_iters++;
		dir = (dir == 4 ? 0 : dir + 1);
		p2 = point_after_moving(p, dir);
	}
	
	return dir;
}

void generate_maze(graph *graph) {

    // Some basic set up.
    const int16_t WIDTH = graph->width;
    const int32_t NUM_CELLS = WIDTH * WIDTH;

    // Get a random starting position.
    srand(time(NULL));
    graph->start_pos = make_point(rand() % WIDTH, rand() % WIDTH);

    // Use an array as a bitset to keep track of which cells have been visited.
    char visited[NUM_CELLS];
    for (int i = 0; i < NUM_CELLS; i++) visited[i] = 0;

    // Use an array as a stack, to keep track of which nodes are to be visited next.
    POINT stack[NUM_CELLS];
    int32_t head = 0;
    stack[head++] = graph->start_pos;
    POINT current;
    POINT last_node_visited;

    while (head != 0) {

        // Pop next node off the stack and mark it as visited. If it hasn't been
        // visited before, update last_node_visited.
        current = stack[head - 1];
        head--;
        if (!has_been_visited(graph, visited, current)) {
            mark_as_visited(graph, visited, current);
            last_node_visited = current;
        }

        // If every neighbour has been visited, we're done processing this node.
        if (all_neighbours_visited(graph, visited, current))
            continue;

        // Pick a random neighbour and connect it to the current node.
        DIRECTION neighbour_dir = random_unvisited_neighbour(graph, visited, current);
        connect_edge(graph, current, neighbour_dir);

        // Put current node back onto the stack, followed by the neighbour.
        stack[head++] = current;
        stack[head++] = point_after_moving(current, neighbour_dir);

    }

    graph->exit_pos = last_node_visited;
}

void pprint_graph(FILE *out, graph *g) {
    fprintf(out, "Pretty printing graph....\n");
    fprintf(out, "Start tile: (%d, %d)\n", g->start_pos.y, g->start_pos.x);
    fprintf(out, "End tile:   (%d, %d)\n", g->exit_pos.y, g->exit_pos.x);
    for (int row = 0; row < g->width; row++) {
        for (int col = 0; col < g->width; col++) {
            TILE t = g->nmap[row * g->width + col];
            fprintf(out, "(%d, %d): ", row, col);
            print_bits(out, t, 4);
            fprintf(out, "\t");
        }
        fprintf(out, "\n");
    }
}
