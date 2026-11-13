#ifndef BFS_H
#define BFS_H

#include "maze.h"
#include <stdbool.h>

#define BFS_MAX_CELLS (MAZE_ROWS * MAZE_COLS)

typedef struct {
    int row;
    int col;
} BFSPosition;

typedef enum {
    BFS_STATUS_IDLE = 0,
    BFS_STATUS_RUNNING,
    BFS_STATUS_FOUND,
    BFS_STATUS_NO_PATH
} BFSStatus;

typedef struct {
    BFSPosition queue[BFS_MAX_CELLS];
    int front;
    int rear;

    bool visited[MAZE_ROWS][MAZE_COLS];
    BFSPosition parent[MAZE_ROWS][MAZE_COLS];
    bool path[MAZE_ROWS][MAZE_COLS];

    BFSPosition current;
    bool hasCurrent;
    int visitedCount;
    int pathLength; /* Number of moves (edges), not number of cells. */
    BFSStatus status;
} BFSState;

/* Clear all search data and return the solver to its idle state. */
void BFSReset(BFSState *state);

/* Initialize BFS at the maze start cell. The search then advances via BFSStep. */
void BFSInit(BFSState *state, const Maze *maze);

/* Process one queue item. Call repeatedly while status is BFS_STATUS_RUNNING. */
void BFSStep(BFSState *state, const Maze *maze);

#endif