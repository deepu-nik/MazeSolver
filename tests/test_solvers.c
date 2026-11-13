#include "maze.h"
#include "bfs.h"
#include "dfs.h"

#include <stdio.h>
#include <stdlib.h>

static int failures = 0;

#define CHECK(condition, message) do { \
    if (!(condition)) { \
        fprintf(stderr, "FAIL: %s (line %d)\n", message, __LINE__); \
        failures++; \
    } \
} while (0)

static bool IsInside(int row, int col) {
    return row >= 0 && row < MAZE_ROWS && col >= 0 && col < MAZE_COLS;
}

static void RunBFS(BFSState *state, const Maze *maze) {
    int steps = 0;
    while (state->status == BFS_STATUS_RUNNING && steps < BFS_MAX_CELLS + 1) {
        BFSStep(state, maze);
        steps++;
    }
    CHECK(state->status != BFS_STATUS_RUNNING, "BFS terminates within the grid size");
}

static void RunDFS(DFSState *state, const Maze *maze) {
    int steps = 0;
    while (state->status == DFS_STATUS_RUNNING && steps < DFS_MAX_CELLS + 1) {
        DFSStep(state, maze);
        steps++;
    }
    CHECK(state->status != DFS_STATUS_RUNNING, "DFS terminates within the grid size");
}

static int CountPathCells(const bool path[MAZE_ROWS][MAZE_COLS]) {
    int count = 0;
    for (int row = 0; row < MAZE_ROWS; row++) {
        for (int col = 0; col < MAZE_COLS; col++) {
            if (path[row][col]) count++;
        }
    }
    return count;
}

static bool IsValidBFSPath(const BFSState *state, const Maze *maze) {
    int row = maze->endRow;
    int col = maze->endCol;
    int moves = 0;

    for (int guard = 0; guard < BFS_MAX_CELLS; guard++) {
        if (!IsInside(row, col) || !state->path[row][col] ||
            maze->cells[row][col] == CELL_WALL) return false;
        if (row == maze->startRow && col == maze->startCol) {
            return moves == state->pathLength &&
                   CountPathCells(state->path) == moves + 1;
        }

        const BFSPosition parent = state->parent[row][col];
        if (!IsInside(parent.row, parent.col) ||
            abs(parent.row - row) + abs(parent.col - col) != 1) return false;
        row = parent.row;
        col = parent.col;
        moves++;
    }
    return false;
}

static bool IsValidDFSPath(const DFSState *state, const Maze *maze) {
    int row = maze->endRow;
    int col = maze->endCol;
    int moves = 0;

    for (int guard = 0; guard < DFS_MAX_CELLS; guard++) {
        if (!IsInside(row, col) || !state->path[row][col] ||
            maze->cells[row][col] == CELL_WALL) return false;
        if (row == maze->startRow && col == maze->startCol) {
            return moves == state->pathLength &&
                   CountPathCells(state->path) == moves + 1;
        }

        const DFSPosition parent = state->parent[row][col];
        if (!IsInside(parent.row, parent.col) ||
            abs(parent.row - row) + abs(parent.col - col) != 1) return false;
        row = parent.row;
        col = parent.col;
        moves++;
    }
    return false;
}

static void TestOpenGrid(void) {
    Maze maze;
    MazeInit(&maze);
    BFSState bfs;
    DFSState dfs;

    BFSInit(&bfs, &maze);
    RunBFS(&bfs, &maze);
    CHECK(bfs.status == BFS_STATUS_FOUND, "BFS finds a route in an open grid");
    CHECK(bfs.pathLength == 28, "BFS open-grid shortest route is 28 moves");
    CHECK(IsValidBFSPath(&bfs, &maze), "BFS open-grid path is connected and avoids walls");

    DFSInit(&dfs, &maze);
    RunDFS(&dfs, &maze);
    CHECK(dfs.status == DFS_STATUS_FOUND, "DFS finds a route in an open grid");
    CHECK(dfs.pathLength >= bfs.pathLength,
          "DFS open-grid route is not shorter than the BFS shortest route");
    CHECK(IsValidDFSPath(&dfs, &maze), "DFS open-grid path is connected and avoids walls");
    printf("Open grid: BFS %d moves / %d visited; DFS %d moves / %d visited\n",
           bfs.pathLength, bfs.visitedCount, dfs.pathLength, dfs.visitedCount);
}

static void TestDetour(void) {
    Maze maze;
    MazeInit(&maze);
    for (int row = 0; row < MAZE_ROWS; row++) maze.cells[row][15] = CELL_WALL;
    maze.cells[MAZE_ROWS - 1][15] = CELL_EMPTY;

    BFSState bfs;
    DFSState dfs;
    BFSInit(&bfs, &maze);
    RunBFS(&bfs, &maze);
    CHECK(bfs.status == BFS_STATUS_FOUND, "BFS finds route through the bottom opening");
    CHECK(bfs.pathLength == 48, "BFS detour route is 48 moves");
    CHECK(IsValidBFSPath(&bfs, &maze), "BFS detour path is connected and avoids walls");

    DFSInit(&dfs, &maze);
    RunDFS(&dfs, &maze);
    CHECK(dfs.status == DFS_STATUS_FOUND, "DFS finds route through the bottom opening");
    CHECK(dfs.pathLength >= bfs.pathLength,
          "DFS detour route is not shorter than the BFS shortest route");
    CHECK(IsValidDFSPath(&dfs, &maze), "DFS detour path is connected and avoids walls");
    printf("Detour: BFS %d moves / %d visited; DFS %d moves / %d visited\n",
           bfs.pathLength, bfs.visitedCount, dfs.pathLength, dfs.visitedCount);
}

static void TestNoRoute(void) {
    Maze maze;
    MazeInit(&maze);
    for (int row = 0; row < MAZE_ROWS; row++) maze.cells[row][15] = CELL_WALL;

    BFSState bfs;
    DFSState dfs;
    BFSInit(&bfs, &maze);
    RunBFS(&bfs, &maze);
    CHECK(bfs.status == BFS_STATUS_NO_PATH, "BFS reports no route behind a complete wall");

    DFSInit(&dfs, &maze);
    RunDFS(&dfs, &maze);
    CHECK(dfs.status == DFS_STATUS_NO_PATH, "DFS reports no route behind a complete wall");
    printf("No route: BFS visited %d; DFS visited %d\n",
           bfs.visitedCount, dfs.visitedCount);
}

static void TestReset(void) {
    Maze maze;
    MazeInit(&maze);
    BFSState bfs;
    DFSState dfs;

    BFSInit(&bfs, &maze);
    RunBFS(&bfs, &maze);
    BFSReset(&bfs);
    CHECK(bfs.status == BFS_STATUS_IDLE, "BFS reset returns to idle");
    CHECK(bfs.visitedCount == 0 && bfs.pathLength == 0,
          "BFS reset clears results");

    DFSInit(&dfs, &maze);
    RunDFS(&dfs, &maze);
    DFSReset(&dfs);
    CHECK(dfs.status == DFS_STATUS_IDLE, "DFS reset returns to idle");
    CHECK(dfs.visitedCount == 0 && dfs.pathLength == 0,
          "DFS reset clears results");
}

int main(void) {
    TestOpenGrid();
    TestDetour();
    TestNoRoute();
    TestReset();

    if (failures > 0) {
        fprintf(stderr, "\n%d test assertion(s) failed.\n", failures);
        return EXIT_FAILURE;
    }

    puts("\nAll BFS and DFS tests passed.");
    return EXIT_SUCCESS;
}
