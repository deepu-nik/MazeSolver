#include "nqueens.h"

#include <stdio.h>
#include <stdlib.h>

static int failures = 0;

#define CHECK(condition, message) do { \
    if (!(condition)) { \
        fprintf(stderr, "FAIL: %s (line %d)\n", message, __LINE__); \
        failures++; \
    } \
} while (0)

int main(void) {
    NQueensResult result;
    NQueensSolve(&result);

    CHECK(result.solved, "8-Queens solver finds a solution");
    CHECK(NQueensValidate(result.solution), "returned arrangement is valid");
    CHECK(result.eventCount > 0, "solver records events for GUI playback");
    CHECK(result.attempts >= NQUEENS_SIZE, "solver records candidate attempts");
    CHECK(result.placements >= NQUEENS_SIZE, "solver places at least eight queens");
    CHECK(result.backtracks >= 0, "backtrack count is non-negative");

    int duplicateColumn[NQUEENS_SIZE] = {0, 0, 2, 3, 4, 5, 6, 7};
    CHECK(!NQueensValidate(duplicateColumn), "validator rejects duplicate columns");

    int diagonalConflict[NQUEENS_SIZE] = {0, 1, 4, 7, 2, 5, 6, 3};
    CHECK(!NQueensValidate(diagonalConflict), "validator rejects diagonal conflicts");

    int outOfRange[NQUEENS_SIZE] = {0, 4, 7, 5, 2, 6, 1, 8};
    CHECK(!NQueensValidate(outOfRange), "validator rejects out-of-range columns");

    if (failures != 0) {
        fprintf(stderr, "%d test(s) failed.\n", failures);
        return EXIT_FAILURE;
    }

    printf("8-Queens solution: ");
    for (int row = 0; row < NQUEENS_SIZE; row++) {
        printf("%d%s", result.solution[row], row == NQUEENS_SIZE - 1 ? "\n" : " ");
    }
    printf("Attempts: %d | Placements: %d | Backtracks: %d\n",
           result.attempts, result.placements, result.backtracks);
    puts("All N-Queens tests passed.");
    return EXIT_SUCCESS;
}
