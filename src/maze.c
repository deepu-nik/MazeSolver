#include "maze.h"

void MazeInit(Maze *maze) {
    if (maze == 0) {
        return;
    }

    for (int row = 0; row < MAZE_ROWS; row++) {
        for (int col = 0; col < MAZE_COLS; col++) {
            maze->cells[row][col] = CELL_EMPTY;
        }
    }

    /* Keep the start and destination fixed for this first milestone. */
    maze->startRow = MAZE_ROWS / 2;
    maze->startCol = 1;
    maze->endRow = MAZE_ROWS / 2;
    maze->endCol = MAZE_COLS - 2;
}

void MazeClearWalls(Maze *maze) {
    if (maze == 0) {
        return;
    }

    for (int row = 0; row < MAZE_ROWS; row++) {
        for (int col = 0; col < MAZE_COLS; col++) {
            maze->cells[row][col] = CELL_EMPTY;
        }
    }
}

void MazeToggleCell(Maze *maze, int row, int col) {
    if (maze == 0 || row < 0 || row >= MAZE_ROWS || col < 0 || col >= MAZE_COLS) {
        return;
    }

    if ((row == maze->startRow && col == maze->startCol) ||
        (row == maze->endRow && col == maze->endCol)) {
        return;
    }

    maze->cells[row][col] =
        (maze->cells[row][col] == CELL_WALL) ? CELL_EMPTY : CELL_WALL;
}

int MazeCountWalls(const Maze *maze) {
    if (maze == 0) {
        return 0;
    }

    int count = 0;
    for (int row = 0; row < MAZE_ROWS; row++) {
        for (int col = 0; col < MAZE_COLS; col++) {
            if (maze->cells[row][col] == CELL_WALL) {
                count++;
            }
        }
    }

    return count;
}
