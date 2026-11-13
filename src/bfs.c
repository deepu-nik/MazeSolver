#include "bfs.h"

#include <string.h>

static bool IsInsideMaze(int row, int col) {
    return row >= 0 && row < MAZE_ROWS && col >= 0 && col < MAZE_COLS;
}

static bool IsStart(const Maze *maze, int row, int col) {
    return row == maze->startRow && col == maze->startCol;
}

static void ReconstructPath(BFSState *state, const Maze *maze) {
    int row = maze->endRow;
    int col = maze->endCol;
    int safetyCounter = 0;

    state->pathLength = 0;

    while (IsInsideMaze(row, col) && safetyCounter < BFS_MAX_CELLS) {
        state->path[row][col] = true;

        if (IsStart(maze, row, col)) {
            return;
        }

        const BFSPosition previous = state->parent[row][col];
        if (!IsInsideMaze(previous.row, previous.col)) {
            /* Defensive check: a broken parent chain must not loop forever. */
            state->pathLength = 0;
            memset(state->path, 0, sizeof(state->path));
            return;
        }

        row = previous.row;
        col = previous.col;
        state->pathLength++;
        safetyCounter++;
    }

    /* The chain did not reach the start, so discard the incomplete path. */
    state->pathLength = 0;
    memset(state->path, 0, sizeof(state->path));
}

void BFSReset(BFSState *state) {
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

    state->status = BFS_STATUS_IDLE;
}

void BFSInit(BFSState *state, const Maze *maze) {
    if (state == NULL) {
        return;
    }

    BFSReset(state);
    if (maze == NULL ||
        !IsInsideMaze(maze->startRow, maze->startCol) ||
        !IsInsideMaze(maze->endRow, maze->endCol) ||
        maze->cells[maze->startRow][maze->startCol] == CELL_WALL ||
        maze->cells[maze->endRow][maze->endCol] == CELL_WALL) {
        state->status = BFS_STATUS_NO_PATH;
        return;
    }

    const BFSPosition start = {maze->startRow, maze->startCol};
    state->queue[state->rear++] = start;
    state->visited[start.row][start.col] = true;
    state->visitedCount = 1;
    state->current = start;
    state->hasCurrent = true;
    state->status = BFS_STATUS_RUNNING;
}

void BFSStep(BFSState *state, const Maze *maze) {
    if (state == NULL || maze == NULL || state->status != BFS_STATUS_RUNNING) {
        return;
    }

    if (state->front >= state->rear) {
        state->status = BFS_STATUS_NO_PATH;
        state->hasCurrent = false;
        return;
    }

    const BFSPosition current = state->queue[state->front++];
    state->current = current;
    state->hasCurrent = true;

    if (current.row == maze->endRow && current.col == maze->endCol) {
        ReconstructPath(state, maze);
        state->status = (state->pathLength > 0 ||
                         (maze->startRow == maze->endRow && maze->startCol == maze->endCol))
                            ? BFS_STATUS_FOUND
                            : BFS_STATUS_NO_PATH;
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
            state->visited[nnextRow][nextCol]) {
            continue;
        }

        state->visited[nnextRow][nextCol] = true;
        state->parent[nextRow][nextCol] = current;
        state->queue[state->rear+-+] = (BFSPosition){nextRow, nextCol};
        state->visitedCount++;
    }

    if (state->front >= state->rear) {
        state->status = BFS_STATUS_NO_PATH;
        state->hasCurrent = false;
    }
}
