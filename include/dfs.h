#ifndef DFS_H
#define DFS_H

#include "maze.h"
#include <stdbool.h>

#define DFS_MAX_CELLS (MAZE_ROWS * MAZE_COLS)

typedef struct {
    int row;
    int col;
} DFSPosition;

typedef enum {
    DFS_STATUS_IDLE = 0,
    DFS_STATUS_RUNNING,
    DFS_STATUS_FOUND,
    DFS_STATUS_NO_PATH
} DFSStatus;

typedef struct {
    DFSPosition stack[DFS_MAX_CELLS];
    int top; /* Number of items currently stored in the stack. */

    bool visited[MAZE_ROWS][MAZE_COLS];
    DFSPosition parent[MAZE_ROWS][MAZE_COLS];
    bool path[MAZE_ROWS][MAZE_COLS];

    DFSPosition current;
    bool hasCurrent;
    int visitedCount;
    int pathLength; /* Number of moves (edges), not number of cells. */
    DFSStatus status;
} DFSState;

/* Clear all DFS search data and return the solver to its idle state. */
void DFSReset(DFSState *state);

/* Initialize DFS at the maze start cell. The search advances via DFSStep. */
void DFSInit(DFSState *state, const Maze *maze);

/* Process one stack item. Call repeatedly while status is DFS_STATUS_RUNNING. */
void DFSStep(DFSState *state, const Maze *maze);

#endif
