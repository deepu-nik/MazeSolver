#ifndef NQUEENS_H
#define NQUEENS_H

#include <stdbool.h>

#define NQUEENS_SIZE 8
/* The first 8-Queens solution needs far fewer than this many trace events. */
#define NQUEENS_MAX_EVENTS 10000

typedef enum {
    NQ_EVENT_TRY = 0,
    NQ_EVENT_PLACE,
    NQ_EVENT_REMOVE
} NQEventType;

typedef struct {
    NQEventType type;
    int row;
    int col;
} NQEvent;

typedef struct {
    int solution[NQUEENS_SIZE];
    NQEvent events[NQUEENS_MAX_EVENTS];
    int eventCount;
    int attempts;
    int placements;
    int backtracks;
    bool solved;
    bool traceOverflow;
} NQueensResult;

void NQueensSolve(NQueensResult *result);
bool NQueensValidate(const int board[NQUEENS_SIZE]);

#endif
