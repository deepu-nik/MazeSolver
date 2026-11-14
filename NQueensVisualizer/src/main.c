#include "nqueens.h"

#include "raylib.h"
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 720
#define BOARD_X 70
#define BOARD_Y 110
#define CELL_SIZE 68
#define BOARD_SIZE (CELL_SIZE * NQUEENS_SIZE)

typedef struct {
    int queens[NQUEENS_SIZE];
    int currentRow;
    int currentCol;
    int eventIndex;
    bool playing;
    bool completed;
    float timer;
} VisualState;

static void ResetVisual(VisualState *visual) {
    memset(visual, 0, sizeof(*visual));
    for (int row = 0; row < NQUEENS_SIZE; row++) {
        visual->queens[row] = -1;
    }
    visual->currentRow = -1;
    visual->currentCol = -1;
}

static void ApplyEvent(VisualState *visual, const NQEvent *event) {
    visual->currentRow = event->row;
    visual->currentCol = event->col;

    if (event->type == NQ_EVENT_PLACE) {
        visual->queens[event->row] = event->col;
    } else if (event->type == NQ_EVENT_REMOVE) {
        visual->queens[event->row] = -1;
    }
}

static void DrawButton(Rectangle bounds, const char *label, Color color) {
    const Vector2 mouse = GetMousePosition();
    const bool hover = CheckCollisionPointRec(mouse, bounds);
    DrawRectangleRounded(bounds, 0.16f, 8, hover ? ColorBrightness(color, 0.12f) : color);
    const int width = MeasureText(label, 20);
    DrawText(label, (int)(bounds.x + (bounds.width - width) / 2),
             (int)(bounds.y + (bounds.height - 20) / 2), 20, RAYWHITE);
}

static void DrawBoard(const VisualState *visual) {
    for (int row = 0; row < NQUEENS_SIZE; row++) {
        for (int col = 0; col < NQUEENS_SIZE; col++) {
            const int x = BOARD_X + col * CELL_SIZE;
            const int y = BOARD_Y + row * CELL_SIZE;
            const bool light = ((row + col) % 2 == 0);
            Color square = light ? (Color){232, 225, 207, 255} : (Color){87, 105, 125, 255};

            if (row == visual->currentRow && col == visual->currentCol) {
                square = (Color){244, 190, 74, 255};
            }

            DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, square);
            DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, (Color){35, 43, 55, 255});

            if (visual->queens[row] == col) {
                const char *queen = "Q";
                const int fontSize = 42;
                const int textWidth = MeasureText(queen, fontSize);
                DrawText(queen, x + (CELL_SIZE - textWidth) / 2,
                         y + (CELL_SIZE - fontSize) / 2 - 2, fontSize,
                         (Color){25, 30, 40, 255});
            }
        }
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "8 Queens - Backtracking Visualizer");
    SetTargetFPS(60);

    NQueensResult result = {0};
    VisualState visual;
    ResetVisual(&visual);

    bool hasSolution = false;
    const Rectangle solveButton = {650, 145, 190, 52};
    const Rectangle stepButton = {650, 212, 190, 52};
    const Rectangle resetButton = {650, 279, 190, 52};

    while (!WindowShouldClose()) {
        const float dt = GetFrameTime();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            const Vector2 mouse = GetMousePosition();

            if (CheckCollisionPointRec(mouse, solveButton)) {
                NQueensSolve(&result);
                ResetVisual(&visual);
                hasSolution = result.solved;
                visual.playing = hasSolution;
            } else if (CheckCollisionPointRec(mouse, stepButton) && hasSolution) {
                visual.playing = false;
                if (visual.eventIndex < result.eventCount) {
                    ApplyEvent(&visual, &result.events[visual.eventIndex++]);
                }
                if (visual.eventIndex >= result.eventCount) {
                    visual.completed = true;
                }
            } else if (CheckCollisionPointRec(mouse, resetButton)) {
                ResetVisual(&visual);
                hasSolution = false;
                memset(&result, 0, sizeof(result));
            }
        }

        if (visual.playing && visual.eventIndex < result.eventCount) {
            visual.timer += dt;
            if (visual.timer >= 0.035f) {
                visual.timer = 0.0f;
                ApplyEvent(&visual, &result.events[visual.eventIndex++]);
            }
            if (visual.eventIndex >= result.eventCount) {
                visual.playing = false;
                visual.completed = true;
            }
        }

        BeginDrawing();
        ClearBackground((Color){16, 22, 33, 255});

        DrawText("8 QUEENS PUZZLE", 70, 32, 34, RAYWHITE);
        DrawText("DAA MINI PROJECT  |  BACKTRACKING", 72, 73, 17, (Color){155, 174, 198, 255});

        DrawBoard(&visual);

        DrawText("CONTROLS", 650, 105, 20, RAYWHITE);
        DrawButton(solveButton, "Solve / Replay", (Color){30, 145, 220, 255});
        DrawButton(stepButton, "Next Step", (Color){117, 86, 190, 255});
        DrawButton(resetButton, "Reset", (Color){62, 75, 94, 255});

        DrawText("LEGEND", 650, 370, 20, RAYWHITE);
        DrawRectangle(652, 407, 18, 18, (Color){232, 225, 207, 255});
        DrawText("Chessboard", 680, 407, 17, RAYWHITE);
        DrawRectangle(652, 438, 18, 18, (Color){244, 190, 74, 255});
        DrawText("Current attempt", 680, 438, 17, RAYWHITE);
        DrawText("Q", 652, 467, 22, RAYWHITE);
        DrawText("Placed queen", 680, 470, 17, RAYWHITE);

        DrawText(TextFormat("Attempts: %d", result.attempts), 650, 535, 18, RAYWHITE);
        DrawText(TextFormat("Placements: %d", result.placements), 650, 565, 18, RAYWHITE);
        DrawText(TextFormat("Backtracks: %d", result.backtracks), 650, 595, 18, RAYWHITE);

        const char *status = "Ready. Select Solve to start.";
        if (visual.playing) status = "Solving: exploring placements...";
        else if (visual.completed && hasSolution) status = "Solution found! All 8 queens are safe.";
        else if (hasSolution && visual.eventIndex > 0) status = "Paused. Select Next Step to continue.";
        DrawText(status, 70, 680, 17, (Color){170, 190, 215, 255});

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
