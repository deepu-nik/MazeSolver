#include "raylib.h"
#include "maze.h"
#include "bfs.h"
#include "dfs.h"

#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 1100
#define WINDOW_HEIGHT 740
#define CELL_SIZE 24
#define GRID_X 40
#define GRID_Y 150
#define SEARCH_STEP_INTERVAL 0.018f

#define SEARCH_NONE 0
#define SEARCH_BFS 1
#define SEARCH_DFS 2

static const Color COLOR_BACKGROUND = {15, 19, 29, 255};
static const Color COLOR_PANEL = {25, 32, 46, 255};
static const Color COLOR_CELL = {37, 47, 64, 255};
static const Color COLOR_CELL_BORDER = {55, 68, 88, 255};
static const Color COLOR_WALL = {5, 8, 14, 255};
static const Color COLOR_START = {42, 190, 112, 255};
static const Color COLOR_END = {226, 76, 86, 255};
static const Color COLOR_TEXT = {235, 240, 248, 255};
static const Color COLOR_MUTED = {155, 168, 187, 255};
static const Color COLOR_ACCENT = {53, 152, 232, 255};
static const Color COLOR_ACCENT_HOVER = {75, 171, 246, 255};
static const Color COLOR_DFS = {118, 93, 190, 255};
static const Color COLOR_VISITED = {49, 111, 178, 255};
static const Color COLOR_PATH = {244, 190, 74, 255};
static const Color COLOR_CURRENT = {153, 105, 236, 255};

static Rectangle GetGridBounds(void) {
    const Rectangle bounds = {
        (float)GRID_X,
        (float)GRID_Y,
        (float)(MAZE_COLS * CELL_SIZE),
        (float)(MAZE_ROWS * CELL_SIZE)
    };
    return bounds;
}

static void DrawButton(Rectangle bounds, const char *label, Vector2 mouse,
                       Color normalColor) {
    const bool hovered = CheckCollisionPointRec(mouse, bounds);
    const Color fill = hovered ? COLOR_ACCENT_HOVER : normalColor;

    DrawRectangleRounded(bounds, 0.18f, 8, fill);
    const int fontSize = 17;
    const int textWidth = MeasureText(label, fontSize);
    DrawText(label,
             (int)(bounds.x + (bounds.width - (float)textWidth) / 2.0f),
             (int)(bounds.y + (bounds.height - (float)fontSize) / 2.0f),
             fontSize,
             COLOR_TEXT);
}

static void DrawMaze(const Maze *maze, const BFSState *bfs,
                     const DFSState *dfs, int activeSearch) {
    for (int row = 0; row < MAZE_ROWS; row++) {
        for (int col = 0; col < MAZE_COLS; col++) {
            const float x = (float)(GRID_X + col * CELL_SIZE);
            const float y = (float)(GRID_Y + row * CELL_SIZE);
            const Rectangle cell = {x, y, (float)CELL_SIZE, (float)CELL_SIZE};
            Color fill = COLOR_CELL;
            bool isVisited = false;
            bool isPath = false;
            bool isCurrent = false;

            if (activeSearch == SEARCH_BFS) {
                isVisited = bfs->visited[row][col];
                isPath = bfs->path[row][col];
                isCurrent = bfs->status == BFS_STATUS_RUNNING &&
                            bfs->hasCurrent && bfs->current.row == row &&
                            bfs->current.col == col;
            } else if (activeSearch == SEARCH_DFS) {
                isVisited = dfs->visited[row][col];
                isPath = dfs->path[row][col];
                isCurrent = dfs->status == DFS_STATUS_RUNNING &&
                            dfs->hasCurrent && dfs->current.row == row &&
                            dfs->current.col == col;
            }

            if (maze->cells[row][col] == CELL_WALL) {
                fill = COLOR_WALL;
            } else if (isPath) {
                fill = COLOR_PATH;
            } else if (isCurrent) {
                fill = COLOR_CURRENT;
            } else if (isVisited) {
                fill = COLOR_VISITED;
            }

            if (row == maze->startRow && col == maze->startCol) {
                fill = COLOR_START;
            } else if (row == maze->endRow && col == maze->endCol) {
                fill = COLOR_END;
            }

            DrawRectangleRec(cell, fill);
            DrawRectangleLinesEx(cell, 1.0f, COLOR_CELL_BORDER);

            if (row == maze->startRow && col == maze->startCol) {
                DrawText("S", (int)x + 7, (int)y + 4, 16, COLOR_TEXT);
            } else if (row == maze->endRow && col == maze->endCol) {
                DrawText("E", (int)x + 7, (int)y + 4, 16, COLOR_TEXT);
            }
        }
    }
}

static void DrawLegendItem(int x, int y, Color color, const char *label) {
    DrawRectangle(x, y, 18, 18, color);
    DrawRectangleLines(x, y, 18, 18, COLOR_CELL_BORDER);
    DrawText(label, x + 29, y + 1, 15, COLOR_TEXT);
}

static void UpdateBFSStatus(char *status, size_t statusSize,
                            const BFSState *bfs) {
    if (bfs->status == BFS_STATUS_FOUND) {
        snprintf(status, statusSize,
                 "BFS complete: shortest path is %d moves (%d visited cells).",
                 bfs->pathLength, bfs->visitedCount);
    } else if (bfs->status == BFS_STATUS_NO_PATH) {
        snprintf(status, statusSize,
                 "BFS: no path found (%d visited cells).", bfs->visitedCount);
    }
}

static void UpdateDFSStatus(char *status, size_t statusSize,
                            const DFSState *dfs) {
    if (dfs->status == DFS_STATUS_FOUND) {
        snprintf(status, statusSize,
                 "DFS found a %d-move path (%d visited). It is not guaranteed shortest.",
                 dfs->pathLength, dfs->visitedCount);
    } else if (dfs->status == DFS_STATUS_NO_PATH) {
        snprintf(status, statusSize,
                 "DFS: no path found (%d visited cells).", dfs->visitedCount);
    }
}

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Maze Solver - DAA Mini Project");
    SetTargetFPS(60);

    Maze maze;
    MazeInit(&maze);

    BFSState bfs;
    BFSReset(&bfs);

    DFSState dfs;
    DFSReset(&dfs);

    int activeSearch = SEARCH_NONE;
    float stepTimer = 0.0f;

    const Rectangle gridBounds = GetGridBounds();
    const Rectangle bfsButton = {820.0f, 158.0f, 240.0f, 42.0f};
    const Rectangle dfsButton = {820.0f, 208.0f, 240.0f, 42.0f};
    const Rectangle resetSearchButton = {820.0f, 258.0f, 240.0f, 42.0f};
    const Rectangle clearButton = {820.0f, 308.0f, 240.0f, 42.0f};

    char status[180] = "Editor ready. Add walls, then choose BFS or DFS.";
    char resultText[100];
    char wallText[48];

    while (!WindowShouldClose()) {
        const Vector2 mouse = GetMousePosition();
        const float frameTime = GetFrameTime();

        if (activeSearch == SEARCH_BFS && bfs.status == BFS_STATUS_RUNNING) {
            stepTimer += frameTime;
            if (stepTimer >= SEARCH_STEP_INTERVAL) {
                stepTimer -= SEARCH_STEP_INTERVAL;
                BFSStep(&bfs, &maze);
                if (bfs.status == BFS_STATUS_FOUND ||
                    bfs.status == BFS_STATUS_NO_PATH) {
                    UpdateBFSStatus(status, sizeof(status), &bfs);
                }
            }
        } else if (activeSearch == SEARCH_DFS &&
                   dfs.status == DFS_STATUS_RUNNING) {
            stepTimer += frameTime;
            if (stepTimer >= SEARCH_STEP_INTERVAL) {
                stepTimer -= SEARCH_STEP_INTERVAL;
                DFSStep(&dfs, &maze);
                if (dfs.status == DFS_STATUS_FOUND ||
                    dfs.status == DFS_STATUS_NO_PATH) {
                    UpdateDFSStatus(status, sizeof(status), &dfs);
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, bfsButton)) {
                DFSReset(&dfs);
                BFSInit(&bfs, &maze);
                activeSearch = SEARCH_BFS;
                stepTimer = 0.0f;

                if (bfs.status == BFS_STATUS_RUNNING) {
                    snprintf(status, sizeof(status),
                             "BFS started. Exploring the maze...");
                } else {
                    UpdateBFSStatus(status, sizeof(status), &bfs);
                }
            } else if (CheckCollisionPointRec(mouse, dfsButton)) {
                BFSReset(&bfs);
                DFSInit(&dfs, &maze);
                activeSearch = SEARCH_DFS;
                stepTimer = 0.0f;

                if (dfs.status == DFS_STATUS_RUNNING) {
                    snprintf(status, sizeof(status),
                             "DFS started. Exploring the maze...");
                } else {
                    UpdateDFSStatus(status, sizeof(status), &dfs);
                }
            } else if (CheckCollisionPointRec(mouse, resetSearchButton)) {
                BFSReset(&bfs);
                DFSReset(&dfs);
                activeSearch = SEARCH_NONE;
                stepTimer = 0.0f;
                snprintf(status, sizeof(status),
                         "Search display reset. Your walls are unchanged.");
            } else if (CheckCollisionPointRec(mouse, clearButton)) {
                MazeClearWalls(&maze);
                BFSReset(&bfs);
                DFSReset(&dfs);
                activeSearch = SEARCH_NONE;
                stepTimer = 0.0f;
                snprintf(status, sizeof(status),
                         "Maze cleared. Start and destination are unchanged.");
            } else if (CheckCollisionPointRec(mouse, gridBounds)) {
                const int col = (int)((mouse.x - (float)GRID_X) / (float)CELL_SIZE);
                const int row = (int)((mouse.y - (float)GRID_Y) / (float)CELL_SIZE);
                MazeToggleCell(&maze, row, col);
                BFSReset(&bfs);
                DFSReset(&dfs);
                activeSearch = SEARCH_NONE;
                stepTimer = 0.0f;
                snprintf(status, sizeof(status),
                         "Maze updated. Previous search results were reset.");
            }
        }

        BeginDrawing();
        ClearBackground(COLOR_BACKGROUND);

        DrawText("MAZE SOLVER", 40, 35, 34, COLOR_TEXT);
        DrawText("DAA MINI PROJECT  |  BFS + DFS GRAPH SEARCH",
                 42, 78, 16, COLOR_MUTED);
        DrawText("EDITABLE GRID  (21 x 31)", GRID_X, 118, 16, COLOR_MUTED);

        DrawMaze(&maze, &bfs, &dfs, activeSearch);

        DrawText("CONTROLS", 820, 119, 18, COLOR_TEXT);
        DrawButton(bfsButton, "Solve using BFS", mouse, COLOR_ACCENT);
        DrawButton(dfsButton, "Solve using DFS", mouse, COLOR_DFS);
        DrawButton(resetSearchButton, "Reset Search", mouse, COLOR_PANEL);
        DrawButton(clearButton, "Clear Walls", mouse, COLOR_PANEL);

        DrawRectangleRounded((Rectangle){820.0f, 360.0f, 240.0f, 90.0f},
                             0.08f, 8, COLOR_PANEL);
        DrawText("HOW TO USE", 838, 372, 17, COLOR_TEXT);
        DrawText("1. Click cells to add/remove walls.", 838, 396, 12, COLOR_MUTED);
        DrawText("2. Choose BFS or DFS to search.", 838, 416, 12, COLOR_MUTED);
        DrawText("3. Blue = visited; yellow = path.", 838, 436, 12, COLOR_MUTED);

        DrawRectangleRounded((Rectangle){820.0f, 460.0f, 240.0f, 160.0f},
                             0.08f, 8, COLOR_PANEL);
        DrawText("LEGEND", 838, 474, 17, COLOR_TEXT);
        DrawLegendItem(840, 500, COLOR_START, "Start");
        DrawLegendItem(840, 523, COLOR_END, "Destination");
        DrawLegendItem(840, 546, COLOR_WALL, "Wall");
        DrawLegendItem(840, 569, COLOR_VISITED, "Visited");
        DrawLegendItem(840, 592, COLOR_PATH, "Path");

        snprintf(wallText, sizeof(wallText), "Walls placed: %d",
                 MazeCountWalls(&maze));
        DrawText(wallText, 820, 628, 15, COLOR_TEXT);

        if (activeSearch == SEARCH_BFS) {
            if (bfs.status == BFS_STATUS_FOUND) {
                snprintf(resultText, sizeof(resultText),
                         "BFS: %d visited | %d moves",
                         bfs.visitedCount, bfs.pathLength);
            } else if (bfs.status == BFS_STATUS_NO_PATH) {
                snprintf(resultText, sizeof(resultText),
                         "BFS: no path | %d visited", bfs.visitedCount);
            } else {
                snprintf(resultText, sizeof(resultText),
                         "BFS (Queue): %d visited", bfs.visitedCount);
            }
            DrawText(resultText, 820, 650, 14, COLOR_MUTED);
        } else if (activeSearch == SEARCH_DFS) {
            if (dfs.status == DFS_STATUS_FOUND) {
                snprintf(resultText, sizeof(resultText),
                         "DFS: %d visited | %d moves",
                         dfs.visitedCount, dfs.pathLength);
            } else if (dfs.status == DFS_STATUS_NO_PATH) {
                snprintf(resultText, sizeof(resultText),
                         "DFS: no path | %d visited", dfs.visitedCount);
            } else {
                snprintf(resultText, sizeof(resultText),
                         "DFS (Stack): %d visited", dfs.visitedCount);
            }
            DrawText(resultText, 820, 650, 14, COLOR_MUTED);
        } else {
            DrawText("Choose BFS or DFS to solve.", 820, 650, 14, COLOR_MUTED);
        }

        DrawText(status, 40, 683, 13, COLOR_MUTED);
        DrawText("ESC: Exit", 980, 683, 14, COLOR_MUTED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
