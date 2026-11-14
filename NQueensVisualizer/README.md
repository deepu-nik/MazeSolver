# 8 Queens Puzzle Visualizer – DAA Mini Project

A small C desktop GUI that demonstrates the **Backtracking** strategy by solving the classic 8-Queens puzzle.

## Scope

- Fixed 8 × 8 chessboard.
- Place exactly eight queens so no two queens share a row, column or diagonal.
- Recursive backtracking algorithm.
- Visual trace of candidate attempts, queen placements and removals (backtracking).
- Solve / Replay, Next Step and Reset controls.
- Displays candidate attempts, successful placements and backtracks.
- Automated tests for solution validity and invalid board arrangements.

The project intentionally stays small. It does not include user-selected board sizes, multiple-solution browsing, timers, save/load or other advanced features.

## Algorithm

The program places one queen in each row. For the current row, it tries columns from left to right. A position is safe if no previously placed queen shares its column or either diagonal. If a safe position is found, the algorithm places the queen and recursively solves the next row. If later rows cannot be completed, the algorithm removes the current queen and tries the next column.

## Complexity

For the standard N-Queens backtracking search, a commonly stated upper bound is O(N!) time because rows are assigned one at a time and previously used columns are excluded. Safety checks take O(N) in this straightforward implementation, so a more explicit conservative bound for this implementation is O(N · N!) time. The recursion/board storage is O(N); the recorded GUI trace uses additional O(T) memory where T is the number of recorded events. For this fixed 8 × 8 application, N is 8.

## Build

CMake fetches Raylib 6.2 on the first configure, so an internet connection is required for the initial dependency download.

### macOS

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j4
./build/NQueensVisualizer
```

### Windows (Visual Studio generator)

```powershell
cmake -S . -B build
cmake --build build --config Debug
.\build\Debug\NQueensVisualizer.exe
```

### Automated tests

```bash
ctest --test-dir build --output-on-failure
```

The test target does not require opening the GUI, although the first CMake configure still fetches Raylib because it is part of the same project configuration.
