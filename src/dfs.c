#include "dfs.h"

#include <string.h>

static bool IsInsideMaze(int row, int col) {
    return row >= 0 && row < MAZE_ROWS && col >= 0 && col < MAZE_COLS;
}

static bool IsStart(const Maze *maze, int row, int col) {
    return row == maze->startRow && col == maze->startCol;
}

static void ReconstructPath(DFSState *state, const Maze *maze) {
    int row = maze->endRow;
    int col = maze->endCol;
    int safetyCounter = 0;

    state->pathLength = 0;

    while (IsInsideMaze(row, col) && safetyCounter < DFS_MAX_CELLS) {
        state->path[row][col] = true;

        if (IsStart(maze, row, col)) {
            return;
        }

        const DFSPosition previous = state->parent[row][col];
        if (!IsInsideMaze(previous.row, previous.col)) {
            state->pathLength = 0;
            memset(state->path, 0, sizeof(state->path));
            return;
        }

        row = previous.row;
        col = previous.col;
        state->pathLength++;
        safetyCounter++;
    }

    /* Discard an incomplete chain if it did not reach the start. */
    state->pathLength = 0;
    memset(state->path, 0, sizeof(state->path));
}

void DFSReset(DFSState *state) {
    if (state == NULL) {
        return;
    }

    memset(state, 0, sizeof(*state));

    for (int row = 0; row < MAZE_ROWS; row++) {
        for (int col = 0; col < MAZE_COLS; col++) {
            state->parent[row][col].row = -1;
            state->parent[row][col].col = -1;
        }
    }

    state->status = DFS_STATUS_IDLE;
}

void DFSInit(DFSState *state, const Maze *maze) {
    if (state == NULL) {
        return;
    }

    DFSReset(state);

    if (maze == NULL ||
        !IsInsideMaze(maze->startRow, maze->startCol) ||
        !IsInsideMaze(maze->endRow, maze->endCol) ||
        maze->cells[maze->startRow][maze->startCol] == CELL_WALL ||
        maze->cells[maze->endRow][maze->endCol] == CELL_WALL) {
        state->status = DFS_STATUS_NO_PATH;
        return;
    }

    const DFSPosition start = {maze->startRow, maze->startCol};
    state->stack[state->top++] = start;
    state->visited[start.row][start.col] = true;
    state->visitedCount = 1;
    state->current = start;
    state->hasCurrent = true;
    state->status = DFS_STATUS_RUNNING;
}

void DFSStep(DFSState *state, const Maze *maze) {
    if (state == NULL || maze == NULL || state->status != DFS_STATUS_RUNNING) {
        return;
    }

    if (state->top <= 0) {
        state->status = DFS_STATUS_NO_PATH;
        state->hasCurrent = false;
        return;
    }

    /* LIFO: the most recently discovered cell is explored first. */
    const DFSPosition current = state->stack[--state->top];
    state->current = current;
    state->hasCurrent = true;

    if (current.row == maze->endRow && current.col == maze->endCol) {
        ReconstructPath(state, maze);
        state->status = (state->pathLength > 0 ||
                         (maze->startRow == maze->endRow &&
                          maze->startCol == maze->endCol))
                            ? DFS_STATUS_FOUND
                            : DFS_STATUS_NO_PATH;
        return;
    }

    /* Four-directional movement: up, right, down, left. */
    static const int rowOffsets[4] = {-1, 0, 1, 0};
    static const int colOffsets[4] = {0, 1, 0, -1};

    for (int direction = 0; direction < 4; direction++) {
        const int nextRow = current.row + rowOffsets[direction];
        const int nextCol = current.col + colOffsets[direction];

        if (!IsInsideMaze(nextRow, nextCol) ||
            maze->cells[nextRow][nextCol] == CELL_WALL ||
            state->visited[nextRow][nextCol]) {
            continue;
        }

        /*
         * Mark when pushing so each cell enters the stack only once.
         * Therefore top can never exceed the number of maze cells.
         */
        state->visited[nextRow][nextCol] = true;
        state->parent[nextRow][nextCol] = current;
        state->stack[state->top++] = (DFSPosition){nextRow, nextCol};
        state->visitedCount++;
    }

    if (state->top <= 0) {
        state->status = DFS_STATUS_NO_PATH;
        state->hasCurrent = false;
    }
}
