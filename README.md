# Maze Solver – DAA Mini Project

A small cross-platform C + Raylib desktop application for demonstrating graph search on a grid maze.

## Current features

- 21 x 31 editable maze grid
- Fixed start (`S`) and destination (`E`) cells
- Left-click an empty cell to place a wall; click a wall to remove it
- Clear Walls and Reset Search controls
- Breadth-First Search (BFS) using an array-based queue
- Step-by-step visualization of discovered cells
- Shortest-path reconstruction and highlighting
- Visited-cell count and shortest-path length display

## DAA strategy

The maze is modeled as an **unweighted graph**. Each traversable cell is a vertex, and an edge connects two cells that are directly adjacent horizontally or vertically. Diagonal movement is not allowed.

BFS uses a FIFO queue and visits vertices in increasing distance from the start. Therefore, when it reaches the destination, the reconstructed route contains the minimum number of moves for this unweighted grid. The `parent` array stores the predecessor of each discovered cell and is used to reconstruct the route.

Let `V` be the number of traversable cells and `E` the number of valid neighbor connections. BFS has **O(V + E) time** and **O(V) auxiliary space**. On a four-neighbor grid, `E` is at most `4V`, so the search is **O(R x C)** for a grid with `R` rows and `C` columns.

## macOS setup

Install Apple's command line developer tools and CMake if they are not already installed:

```bash
xcode-select --install
brew install cmake
```

Build and run from the project directory:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/MazeSolver
```

On some macOS 27.0 Command Line Tools installations, the included linker may not understand the new `arm64e.x1` architecture entry in the macOS 27 SDK. On the affected setup, select the installed macOS 26.5 SDK explicitly (as done during development):

```bash
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DCMAKE_OSX_SYSROOT=/Library/Developer/CommandLineTools/SDKs/MacOSX26.5.sdk
cmake --build build -j4
./build/MazeSolver
```

The first CMake configure downloads Raylib 6.2 from its official repository, so an internet connection is needed during the initial configure.

## Windows setup

Install CMake and a C compiler toolchain. Visual Studio Community with **Desktop development with C++** is one option. Open a Developer PowerShell/Developer Command Prompt in this folder, then run:

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

With the Visual Studio generator, run:

```powershell
.\build\Debug\MazeSolver.exe
```

## Controls

- **Solve using BFS**: run BFS and animate the search.
- **Reset Search**: clear visited cells and the displayed path without changing walls.
- **Clear Walls**: remove all walls and clear the search result.
- **Grid click**: add or remove a wall; the previous search result is reset.
- **Esc**: close the application.

## Project scope

The required implementation at this stage is BFS. DFS may be added later as a comparison algorithm if the project timeline allows.

## Suggested BFS test cases

1. **Open grid:** leave all cells empty. The route from the fixed start to destination should be 28 moves.
2. **Detour required:** create a vertical wall in column 15, leaving only the bottom-row cell open. BFS should find the detour in 48 moves.
3. **No route:** create a complete vertical wall in column 15. BFS should report that no path exists.

These cases cover a direct shortest route, a route that must go around obstacles, and an unreachable destination.
