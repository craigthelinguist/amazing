
#include "graph.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

void terminate(const char *err_msg) {
	fprintf(stderr, err_msg);
	exit(1);
}

struct Graph {
	int16_t width;	// can be at most 256
	POINT startPos;	// start of the maze
	POINT exitPos;	// you win if you get here
	int16_t nmap[];	// a 2d array of neighbours
};

int16_t GRAPH_width(struct Graph *g) { return g->width; }

int16_t GRAPH_tileAt(struct Graph *g, int16_t xpos, int16_t ypos) {
	return g->nmap[xpos * GRAPH_width(g) + ypos];
}

POINT GRAPH_StartPos(struct Graph *g) { return g->startPos; }
POINT GRAPH_ExitPos (struct Graph *g) { return g->exitPos;  }

int16_t GRAPH_isEntryOrExit(struct Graph *g, int16_t xpos, int16_t ypos) {
	return (g->startPos.x = xpos && g->startPos.y == ypos)
		|| (g->exitPos.x = xpos && g->exitPos.y == ypos);
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

void generate_maze(struct Graph *g);

struct Graph *GRAPH_Make(int16_t width, MazeAlgo generation_algorithm) {
	struct Graph *g = malloc(sizeof(struct Graph) + sizeof(int16_t) * width * width);
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

void GRAPH_Free(struct Graph *graph) {
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

void check_point_in_bounds(struct Graph *g, POINT p) {	
	if (!in_bounds(g, p)) {
		fprintf(stderr, "Point (%d, %d) is out of bounds.", p.x, p.y);
		exit(1);
	}
}

void connect(struct Graph *g, POINT p, enum Direction dir) {
	check_point_in_bounds(g, p);
	g->nmap[p.x * g->width + p.y] |= (1 << dir);
	p = point_after_moving(p, dir);
	check_point_in_bounds(g, p);
	g->nmap[p.x * g->width + p.y] |= (1 << opposite_direction(dir));
}

int16_t is_connected(TILE nmap, enum Direction dir) {
	return !!(nmap & (1 << dir));
}

int16_t is_connected2(struct Graph *g, POINT p, enum Direction dir) {
	TILE t = GRAPH_tileAt(g, p.x, p.y);
	return !!(t & (1 << dir));
}

int has_been_visited(struct Graph *g, char *visited, POINT p) {
	return visited[g->width * p.x + p.y];
}

int all_neighbours_visited(struct Graph *g, char *visited, POINT p) {
	for (DIRECTION dir = 0; dir < 4; dir++) {
		POINT p2 = point_after_moving(p, dir);
		if (in_bounds(g, p2) && !has_been_visited(g, visited, p2))
			return 0;
	}
	return 1;
}

void mark_as_visited(struct Graph *g, char *visited, POINT p) {
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

void generate_maze(struct Graph *graph) {

	// Some basic set up.
	const int16_t WIDTH = graph->width;
	const int32_t NUM_CELLS = WIDTH * WIDTH;
	
	// Get a random starting position.
	srand(time(NULL));
	graph->startPos = make_point(rand() % WIDTH, rand() % WIDTH);
	
	// Use an array as a bitset to keep track of which cells have been visited.
	char visited[NUM_CELLS];
	for (int i = 0; i < NUM_CELLS; i++) visited[i] = 0;
	
	// Use an array as a stack, to keep track of which nodes are to be visited next.
	POINT stack[NUM_CELLS];
	int32_t head = 0;
	stack[head++] = graph->startPos;
	POINT current;
	POINT last_node_visited;

	while (head != 0) {
		
		// Pop next node off the stack and mark it as visited. If it hasn't been
		// visited before, update last_node_visited.
		current = stack[head-1];
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
		connect(graph, current, neighbour_dir);
		
		// Put current node back onto the stack, followed by the neighbour.
		stack[head++] = current;
		stack[head++] = point_after_moving(current, neighbour_dir);
		
	}
	
	graph->exitPos = last_node_visited;
}
