

#ifndef GAME_STATE0_H
#define GAME_STATE0_H

#include "camera.h"
#include "entity0.h"
#include "game_state.h"
#include "graph.h"

/// The maximum number of entities allowed in the game at once.
#define MAX_ENTITIES 10

struct GameState {
    Camera camera;
    GRAPH graph;
    Entity entities[MAX_ENTITIES];
};

#endif

