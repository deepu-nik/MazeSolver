# 8 Queens Puzzle Visualizer – DAA Mini Project Report

**Programme:** SY B.Tech – Computer Science and Engineering  
**Academic Year:** 2026–2027  
**DAA Strategy:** Backtracking  
**Programming Language:** C  
**GUI Library:** Raylib 6.2  
**Build System:** CMake

> Fill in the three student names, PRNs, project guide and signatures before submission. Insert genuine screenshots from the running application and actual CI/test output. Do not treat pending CI as a passed test.

## Team Details

| Sr. No. | Student Name | PRN / Roll No. |
|---|---|---|
| 1 | ______________________________ | __________________ |
| 2 | ______________________________ | __________________ |
| 3 | ______________________________ | __________________ |

**Project Guide:** __________________________________

## Abstract

The 8 Queens Puzzle Visualizer is a graphical desktop application developed in the C programming language. It demonstrates the Backtracking strategy by solving the classic 8-Queens problem. The objective is to place eight queens on an 8 × 8 chessboard so that no two queens attack one another. Therefore, no two queens may share the same row, column or diagonal.

The application places one queen in each row using recursive backtracking. For each row, it tries candidate columns from left to right and checks whether the candidate is safe with respect to the queens already placed in earlier rows. If a safe position is found, the algorithm places the queen and recursively proceeds to the next row. If a later row has no valid position, the algorithm removes the previously placed queen and tries another column.

The GUI visualizes candidate attempts, successful placements and removals. It also displays the number of candidate attempts, placements and backtracks. The application is intentionally limited to a fixed 8 × 8 board and displays one valid solution, keeping the project appropriate for a DAA minor project.

**Keywords:** Design and Analysis of Algorithms, Backtracking, N-Queens, Recursion, Constraint Satisfaction, C, Raylib.

## 1. Introduction

The N-Queens problem is a classic combinatorial problem in which N queens must be placed on an N × N chessboard so that no two queens attack each other. A queen in chess can move horizontally, vertically and diagonally. For the 8-Queens problem, eight queens must be placed on an 8 × 8 board while satisfying these restrictions.

Trying every possible arrangement directly is inefficient. Backtracking improves the search by building a solution incrementally and abandoning a partial arrangement as soon as it violates a constraint. The project demonstrates this strategy through a visual desktop application.

### 1.1 Relevance to DAA

Backtracking is a systematic search technique. It constructs a candidate solution step by step. Whenever a partial solution cannot lead to a valid complete solution, the algorithm reverses the most recent choice and explores another possibility. The N-Queens problem is a clear example of this technique.

## 2. Problem Statement and Objectives

### 2.1 Problem Statement

Design and implement a GUI-based application in C that solves the fixed 8-Queens problem using recursive backtracking. The application must place eight queens on an 8 × 8 chessboard such that no two queens share a row, column or diagonal, and visually demonstrate the placement and backtracking process.

### 2.2 Objectives

- Understand and implement the Backtracking strategy.
- Place exactly one queen in each row of an 8 × 8 board.
- Check column and diagonal constraints before placing a queen.
- Recursively explore candidate positions.
- Backtrack when a partial placement cannot be completed.
- Display one valid solution on a graphical chessboard.
- Visualize candidate attempts, placements and removals.
- Validate the final arrangement through automated tests.
- Analyse time and space complexity.

### 2.3 Expected Outcome

The application displays a valid arrangement of eight queens and visually demonstrates how recursive backtracking explores candidate cells, places queens and removes choices when necessary.

## 3. Literature Review / Existing Approaches

The N-Queens problem can be approached using different search methods.

| Approach | Description | Limitation / Strength |
|---|---|---|
| Brute force | Examines possible arrangements and checks constraints after constructing them. | Very large search space; performs unnecessary checks on invalid partial arrangements. |
| Backtracking | Checks constraints after each placement and abandons invalid partial arrangements immediately. | Still has exponential/factorial search growth for increasing N, but prunes many candidates. |
| Optimized backtracking | Uses column and diagonal lookup arrays to check safety quickly. | Requires additional bookkeeping arrays. |

The project uses straightforward recursive backtracking. Its safety check compares a candidate with queens already placed in previous rows. This keeps the implementation easy to understand and explain in a viva.

## 4. System Requirements and Scope

### 4.1 Hardware Requirements

- Desktop or laptop computer.
- Keyboard and mouse or trackpad.
- Display capable of running a desktop graphical application.

### 4.2 Software Requirements

| Tool | Purpose |
|---|---|
| C language (C11) | Algorithm and GUI implementation. |
| Raylib 6.2 | Window, drawing, text and graphical interface. |
| CMake 3.16 or newer | Cross-platform build configuration. |
| C compiler | Apple Clang, GCC, Clang or a compatible Windows toolchain. |
| CTest | Automated test execution. |

### 4.3 Functional Requirements

1. Display an 8 × 8 chessboard.
2. Solve the fixed 8-Queens problem using recursive backtracking.
3. Show candidate positions and queen placements.
4. Show removal of queens during backtracking.
5. Provide Solve / Replay, Next Step and Reset controls.
6. Display candidate attempts, placements and backtrack counts.
7. Display a completion message when a valid solution is reached.

### 4.4 Scope and Assumptions

- Board size is fixed at 8 × 8.
- The application displays one valid solution.
- Exactly one queen is placed in each row.
- Candidate columns are considered from left to right.
- A position is safe only when it does not share a column or diagonal with an already placed queen.
- User-defined board sizes, multiple-solution browsing, save/load and advanced optimization are outside the current scope.

## 5. Selected DAA Strategy: Backtracking

### 5.1 Concept

Backtracking constructs a solution one decision at a time. If a decision makes the partial solution invalid or prevents completion, that decision is undone and another choice is attempted.

For N-Queens, the algorithm processes rows in order. It attempts to place one queen in the current row. Since earlier rows already contain queens, only the column and diagonal conflicts with those earlier queens need to be checked.

### 5.2 Safety Conditions

A queen at position (row, col) is safe if, for every previously occupied row (previousRow, previousCol):

- Column condition: previousCol ≠ col.
- Main diagonal condition: previousRow − previousCol ≠ row − col.
- Other diagonal condition: previousRow + previousCol ≠ row + col.

The row condition is automatically satisfied because the algorithm places only one queen in each row.

### 5.3 Recursive Backtracking

1. If the current row equals 8, all eight queens have been placed; a solution is found.
2. Try each column from 0 to 7 in the current row.
3. Record the candidate attempt.
4. Check whether the position is safe.
5. If safe, place the queen and recursively solve the next row.
6. If the recursive call succeeds, return success.
7. Otherwise remove the queen and try the next column.
8. If no column works, return failure to the previous recursive call.

## 6. Algorithm, Pseudocode and Flowchart

### 6.1 Pseudocode

```text
NQueens(row):
    if row == 8:
        return SUCCESS

    for col = 0 to 7:
        if IsSafe(row, col):
            place queen at (row, col)

            if NQueens(row + 1) == SUCCESS:
                return SUCCESS

            remove queen from (row, col)

    return FAILURE
```

### 6.2 IsSafe Pseudocode

```text
IsSafe(row, col):
    for previousRow = 0 to row - 1:
        previousCol = queen position in previousRow

        if previousCol == col:
            return FALSE

        if previousRow - previousCol == row - col:
            return FALSE

        if previousRow + previousCol == row + col:
            return FALSE

    return TRUE
```

### 6.3 Flowchart – Text Representation

```text
             START
               |
       Initialize empty board
               |
          Solve row 0
               |
       Is row equal to 8?
          /          \
        YES           NO
         |             |
  Solution found   Try next column
         |             |
        STOP       Is position safe?
                    /       \
                  NO         YES
                  |           |
             Try next      Place queen
              column          |
                           Solve next row
                               |
                        Did recursion succeed?
                           /        \
                         YES         NO
                          |           |
                       Return      Remove queen
                       success        |
                                  Try next column
```

### 6.4 Backtracking Flowchart Description

The flowchart begins by initializing an empty board. The algorithm checks whether all eight rows have been processed. If so, it reports success. Otherwise, it tests candidate columns in the current row. Unsafe positions are skipped. Safe positions are temporarily occupied and the next row is processed recursively. If the recursive call fails, the queen is removed and another column is tested. If all columns fail, the algorithm returns to the previous row.

## 7. System Design and Module Description

### 7.1 Architecture

```text
             USER
               |
               v
          Raylib GUI
             main.c
               |
               v
        NQueensSolve()
            nqueens.c
               |
        Recursive search
          /          \
      IsSafe()     Backtrack
          \          /
               |
               v
        Recorded event trace
               |
               v
       GUI animation / metrics
```

### 7.2 Modules

| Module | Responsibility |
|---|---|
| GUI module – src/main.c | Creates the window, draws the chessboard and controls, plays the event trace and displays counters. |
| Algorithm module – src/nqueens.c | Implements recursive backtracking, safety checks, solution generation and validation. |
| Header – include/nqueens.h | Defines board size, event types, result structure and public function declarations. |
| Test module – tests/test_nqueens.c | Validates the solver output and rejects invalid board arrangements. |

### 7.3 Data Structures

- **One-dimensional board array:** solution[row] stores the column occupied by the queen in that row. A value of −1 represents an unoccupied row during search.
- **Recursive call stack:** stores the active row and local execution state during recursive calls.
- **Event array:** stores candidate attempts, placements and removals for GUI playback.
- **Result structure:** stores the solution, event count, attempts, placements, backtracks and solver status.

## 8. Implementation Details

### 8.1 Development Environment

The application is written in C using Raylib 6.2. CMake manages the build configuration and dependency. The code is separated into the GUI, algorithm, header and test modules.

### 8.2 Recursive Solver

The solver initializes all board rows as unoccupied. It calls the recursive function for row zero. The function tries each column, verifies safety against previously placed queens, places a queen when safe and recursively processes the next row. If a recursive call fails, the current queen is removed.

### 8.3 GUI and Event Playback

The solver records three event types:

- TRY: a candidate cell is being considered.
- PLACE: a queen is placed at a safe cell.
- REMOVE: a previously placed queen is removed during backtracking.

The GUI replays these events so that the user can see the algorithm's decisions. Solve / Replay starts the visual solution, Next Step advances one event at a time, and Reset clears the displayed result.

### 8.4 Build Instructions

Raylib is fetched by CMake during the first configuration, so an internet connection is required for the initial dependency download.

**macOS**

```bash
cd NQueensVisualizer
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j4
./build/NQueensVisualizer
```

**Windows (Visual Studio generator)**

```powershell
cd NQueensVisualizer
cmake -S . -B build
cmake --build build --config Debug
.\build\Debug\NQueensVisualizer.exe
```

**Automated tests**

```bash
ctest --test-dir build --output-on-failure
```

## 9. Testing and Results

### 9.1 Test Cases

| Test ID | Test input / condition | Expected result |
|---|---|---|
| TC-01 | Run the solver on an empty 8 × 8 board. | A complete arrangement of eight queens is found. |
| TC-02 | Validate the returned arrangement. | No two queens share a column or diagonal. |
| TC-03 | Validate an arrangement containing duplicate columns. | Validator rejects the arrangement. |
| TC-04 | Validate an arrangement containing a diagonal conflict. | Validator rejects the arrangement. |
| TC-05 | Validate an arrangement containing an out-of-range column. | Validator rejects the arrangement. |
| TC-06 | Play the recorded trace. | Candidate, placement and removal events are displayed in order. |
| TC-07 | Press Reset. | Board and counters return to the initial state. |

### 9.2 Expected Valid Arrangement

The first solution found by the current left-to-right search order is represented by the following zero-based column positions:

```text
Row:       0 1 2 3 4 5 6 7
Column:    0 4 7 5 2 6 1 3
```

The arrangement is valid because each row contains one queen, all column positions are distinct, and no pair of queens lies on the same diagonal.

### 9.3 Test Status and Evidence

The automated test program is included in the repository. GitHub Actions is configured to build and run the application and tests on macOS and Windows. The actual workflow result must be checked before reporting the tests as passed.

Insert genuine evidence before final printing:

- Figure 9.1 – Initial 8 × 8 chessboard.
- Figure 9.2 – Candidate attempt and queen placement during backtracking.
- Figure 9.3 – Queen removal during backtracking.
- Figure 9.4 – Final valid eight-queen arrangement.
- Figure 9.5 – Automated test output / GitHub Actions result.

## 10. Time and Space Complexity Analysis

Let N represent the number of rows, columns and queens. In this application N = 8.

### 10.1 Time Complexity

At each row, the algorithm tries candidate columns. A partial arrangement is abandoned as soon as a conflict is detected. Since each row receives one queen and columns cannot be repeated, the number of candidate arrangements is bounded by permutations of columns, which is O(N!).

In this implementation, IsSafe scans previously filled rows. Its cost is O(N) in the worst case. Therefore, a conservative bound for the actual implementation is O(N · N!).

| Case | Complexity | Explanation |
|---|---|---|
| Best case | Θ(N²) for this implementation | If the first candidate at each row leads directly to a solution, the safety scans across rows take 1 + 2 + ... + (N−1) comparisons, which is Θ(N²). |
| Average case | Input/order dependent; bounded by O(N · N!) | The number of explored partial arrangements depends on how early conflicts prune the search and when the first solution is reached. There is no single universal average without a specified input distribution. |
| Worst case | O(N · N!) | The search may explore a factorial number of candidate arrangements, and each safety check can scan up to N previously placed queens. |

A commonly quoted O(N!) time bound assumes constant-time safety checks using precomputed column and diagonal occupancy arrays. This project uses a simpler linear scan for safety, so O(N · N!) is the more explicit conservative bound for the implemented version.

### 10.2 Space Complexity

| Component | Space |
|---|---|
| Board array | O(N) |
| Recursive call stack | O(N) |
| Local search state | O(N) |
| Auxiliary algorithm space (excluding GUI trace) | O(N) |
| Recorded GUI event trace | O(T), where T is the number of recorded events |

The core recursive backtracking algorithm uses O(N) auxiliary space. The visualizer additionally stores its event trace, which requires O(T) memory. For the fixed N = 8 application, the board and recursion depth are small.

## 11. Conclusion and Future Scope

### 11.1 Conclusion

The 8 Queens Puzzle Visualizer demonstrates the Backtracking strategy through a recursive C implementation and a graphical interface. It places queens row by row, checks column and diagonal constraints, and reverses choices when a partial arrangement cannot lead to a solution. The event trace makes the algorithm's decisions visible and helps explain recursion and backtracking.

The project provides a practical example of a combinatorial search problem and demonstrates the relationship between correctness constraints, pruning, recursion and complexity analysis.

### 11.2 Limitations

- The board size is fixed at 8 × 8.
- The application displays one solution.
- The search order is fixed from left to right.
- The safety check scans previously occupied rows.
- The event trace has a fixed capacity.
- The current version does not support user-defined blocked cells or custom board sizes.

### 11.3 Future Scope

- Allow users to select different board sizes.
- Display multiple solutions.
- Optimize safety checking using column and diagonal lookup arrays.
- Add controls to adjust animation speed.
- Add a solution counter and compare search statistics for different N values.

These are future extensions and are not claimed as completed features.

## 12. References

1. Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest and Clifford Stein, *Introduction to Algorithms*, 4th Edition, MIT Press, 2022. (Backtracking and algorithm analysis concepts.)
2. Steven S. Skiena, *The Algorithm Design Manual*, 3rd Edition, Springer, 2020. (Search and combinatorial problem-solving concepts.)
3. Raylib official website and documentation: https://www.raylib.com/
4. Raylib source repository: https://github.com/raysan5/raylib
5. CMake documentation: https://cmake.org/documentation/
6. ISO C working group resources: https://www.open-std.org/jtc1/sc22/wg14/

## Appendix A – Demonstration Checklist

1. Introduce the 8-Queens problem.
2. Explain why two queens must not share a column or diagonal.
3. Explain how the algorithm places one queen in each row.
4. Demonstrate the IsSafe check.
5. Run the solver and show the candidate attempts.
6. Explain a backtracking removal.
7. Show the final valid arrangement.
8. Explain the recursive call stack and complexity.
9. Show automated test evidence.

## Appendix B – Team Contribution Record

| Team member | Contribution (edit to reflect actual work) |
|---|---|
| Member 1: __________________ | Algorithm implementation and integration. |
| Member 2: __________________ | GUI validation, test cases and result verification. |
| Member 3: __________________ | Documentation, flowchart, complexity analysis and demonstration. |

All team members must participate in the project demonstration and viva as required by the college guidelines.

## Appendix C – Submission Checklist

- Complete all student names and PRNs.
- Add project guide details and required signatures.
- Insert actual application screenshots.
- Verify automated test results.
- Verify Windows build and runtime if cross-platform support is claimed.
- Update page numbers in the final formatted report.
- Print the final report on A4 paper and place it in the required file.
