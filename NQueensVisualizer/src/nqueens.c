#include "nqueens.h"

#include <string.h>

static void AddEvent(NQueensResult *result, NQEventType type, int row, int col) {
    if (result->eventCount >= NQUEENS_MAX_EVENTS) {
        result->traceOverflow = true;
        return;
    }

    result->events[result->eventCount++] = (NQEvent){type, row, col};
}

static bool IsSafe(const int board[NQUEENS_SIZE], int row, int col) {
    for (int previousRow = 0; previousRow < row; previousRow++) {
        const int previousCol = board[previousRow];

        if (previousCol == col) {
            return false;
        }

        if (previousRow - previousCol == row - col ||
            previousRow + previousCol == row + col) {
            return false;
        }
    }

    return true;
}

static bool SolveRow(NQueensResult *result, int board[NQUEENS_SIZE], int row) {
    if (row == NQUEENS_SIZE) {
        return true;
    }

    for (int col = 0; col < NQUEENS_SIZE; col++) {
        result->attempts++;
        AddEvent(result, NQ_EVENT_TRY, row, col);

        if (!IsSafe(board, row, col)) {
            continue;
        }

        board[row] = col;
        result->placements++;
        AddEvent(result, NQ_EVENT_PLACE, row, col);

        if (SolveRow(result, board, row + 1)) {
            return true;
        }

        board[row] = -1;
        result->backtracks++;
        AddEvent(result, NQ_EVENT_REMOVE, row, col);
    }

    return false;
}

void NQueensSolve(NQueensResult *result) {
    if (result == NULL) {
        return;
    }

    memset(result, 0, sizeof(*result));

    int board[NQUEENS_SIZE];
    for (int row = 0; row < NQUEENS_SIZE; row++) {
        board[row] = -1;
        result->solution[row] = -1;
    }

    result->solved = SolveRow(result, board, 0);

    if (result->solved) {
        for (int row = 0; row < NQUEENS_SIZE; row++) {
            result->solution[row] = board[row];
        }
    }
}

bool NQueensValidate(const int board[NQUEENS_SIZE]) {
    if (board == NULL) {
        return false;
    }

    for (int row = 0; row < NQUEENS_SIZE; row++) {
        if (board[row] < 0 || board[row] >= NQUEENS_SIZE) {
            return false;
        }

        for (int previousRow = 0; previousRow < row; previousRow++) {
            const int previousCol = board[previousRow];

            if (previousCol == board[row]) {
                return false;
            }

            if (previousRow - previousCol == row - board[row] ||
                previousRow + previousCol == row + board[row]) {
                return false;
            }
        }
    }

    return true;
}
