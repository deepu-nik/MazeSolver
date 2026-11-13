#ifndef MAZE_H
#define MAZE_H

#define MAZE_ROWS 21
#define MAZE_COLS 31

typedef enum {
    CELL_EMPTY = 0,
    CELL_WALL = 1
} CellType;

typedef struct {
    CellType cells[MAZE_ROWS][MAZE_COLS];
    int startRow;
    int startCol;
    int endRow;
    int endCol;
} Maze;

void MazeInit(Maze *maze);
void MazeClearWalls(Maze *maze);
void MazeToggleCell(Maze *maze, int row, int col);
int MazeCountWalls(const Maze *maze);

#endif
