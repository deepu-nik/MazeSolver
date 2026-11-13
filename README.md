# Maze Solver – DAA Mini Project

A small cross-platform desktop application written in C with Raylib. It demonstrates graph search on an editable grid maze.

## Current scope

- 21 x 31 editable maze grid
- Fixed start (S) and destination (E) cells
- Left-click an empty cell to place a wall; click a wall to remove it
- Breadth-First Search (BFS) using an array-based FIFO queue
- Depth-First Search (DFS) using an array-based LIFO stack
- Step-by-step visualization of discovered cells
- Parent tracking and path reconstruction
- Visited-cell count and path-length display
- Reset Search and Clear Walls controls

The project intentionally stays small for a DAA minor project. A* search, weighted terrain, automatic maze generation, and save/load are outside the current scope.

## DAA strategy: Graph traversal

The maze is modeled as an **unweighted graph**. Each traversable cell is a vertex, and an edge connects two cells that are directly adjacent horizontally or vertically. Diagonal movement is not allowed.

### Breadth-First Search (BFS)

BFS uses a FIFO queue and discovers cells in non-decreasing order of distance from the start. Therefore, the path it finds is a shortest path in this unweighted grid. A parent array stores the predecessor of each discovered cell and is used to reconstruct the route.

### Depth-First Search (DFS)

DFS uses a LIFO stack and explores the most recently discovered cell first. It finds a path if one exists, but **does not guarantee a shortest path**. The same parent-tracking method reconstructs the path found by DFS.

### Complexity analysis

Let V be the number of traversable cells and E the number of valid neighbor connections.

| Algorithm | Best case | Average-case upper bound | Worst case | Auxiliary space |
|---|---|---|---|---|
| BFS | O(1), if start equals destination | O(V + E) | O(V + E) | O(V) |
| DFS | O(1), if start equals destination | O(V + E) | O(V + E) | O(V) |

The average amount of work depends on the maze layout and neighbor order; O(V + E) is the upper bound. Both algorithms visit each reachable cell at most once and inspect at most four neighbors per cell. On a four-neighbor grid, E is at most 4V, so the worst-case search time is O(R x C) for a grid with R rows and C columns.

BFS is optimal for the number of moves in this unweighted maze. DFS is included to compare traversal behavior and is not an optimal-path algorithm.

## Requirements

- CMake 3.16 or newer
- A C compiler (Apple Clang, Clang, or GCC)
- Internet connection for the first CMake configure, which downloads Raylib 6.2

## macOS setup

Install Apple's command-line developer tools and CMake if they are not already installed:

    xcode-select --install
    brew install cmake

On most macOS setups, configure, build, and run with:

    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    ./build/MazeSolver

**SDK note for the affected macOS 27.0 Command Line Tools setup:** the linker bundled with some installations cannot parse the arm64e.x1 architecture entry in the macOS 27 SDK. If CMake reports a TAPI error containing unknown architecture arm64e.x1-macos, use the installed macOS 26.5 SDK explicitly:

    cmake -S . -B build \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_OSX_ARCHITECTURES=arm64 \
      -DCMAKE_OSX_SYSROOT=/Library/Developer/CommandLineTools/SDKs/MacOSX26.5.sdk
    cmake --build build -j4
    ./build/MazeSolver

If you previously configured the project with a different SDK, remove the generated build directory before reconfiguring:

    rm -rf build

## Windows setup

Install CMake and a C compiler toolchain. Visual Studio Community with **Desktop development with C++** is one option. Open a Developer PowerShell or Developer Command Prompt in this folder, then run:

    cmake -S . -B build
    cmake --build build --config Debug

With the Visual Studio generator, run:

    .\build\Debug\MazeSolver.exe

## Controls

- **Solve using BFS**: run BFS and animate the search. BFS returns a shortest path in the unweighted grid.
- **Solve using DFS**: run DFS and animate the search. The path found is not guaranteed to be shortest.
- **Reset Search**: clear visited cells and the displayed path without changing walls.
- **Clear Walls**: remove all walls and clear the search result.
- **Grid click**: add or remove a wall; previous search results are reset.
- **Esc**: close the application.

## Suggested test cases

1. **Open grid:** leave all cells empty. The fixed start and destination are 28 moves apart. BFS should report 28 moves. DFS may report a longer path.
2. **Detour required:** create a vertical wall in column 15, leaving only the bottom-row cell open. BFS should find a 48-move path. DFS should find a valid route, which may be longer.
3. **No route:** create a complete vertical wall in column 15. Both algorithms should report that no path exists.

Run both algorithms on the same maze when comparing their visited-cell counts and path lengths. Record the actual values from the application during testing; results depend on maze layout and neighbor order.
