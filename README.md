# Maze Solver – DAA Mini Project

A small cross-platform desktop application written in C with Raylib. It demonstrates graph search on an editable grid maze.

## Current scope

- 21 x 31 editable maze grid
- Fixed start (`S`) and destination (`E`) cells
- Left-click an empty cell to place a wall; click a wall to remove it
- Breadth-First Search (BFS) using an array-based FIFO queue
- Step-by-step visualization of discovered cells
- Shortest-path reconstruction and highlighting
- Visited-cell count and shortest-path length
- Reset Search and Clear Walls controls

The project intentionally stays small for a DAA minor project. DFS may be added later as a comparison algorithm if required by the team or instructor. Advanced features such as A*, weighted terrain, automatic maze generation, and save/load are outside the current scope.

## DAA strategy: Breadth-First Search

The maze is modeled as an **unweighted graph**. Each traversable cell is a vertex, and an edge connects two cells that are directly adjacent horizontally or vertically. Diagonal movement is not allowed.

BFS uses a FIFO queue and discovers cells in non-decreasing order of distance from the start. Therefore, when it reaches the destination, the reconstructed route contains the minimum number of moves for this unweighted grid. A `parent` array stores the predecessor of each discovered cell and is used to reconstruct the route.

Let `V` be the number of traversable cells and `E` the number of valid neighbor connections. BFS takes **O(V + E) time** and **O(V) auxiliary space**. On a four-neighbor grid, `E` is at most `4V`, so the search is **O(R x C)** for a grid with `R` rows and `C` columns.

## Requirements

- CMake 3.16 or newer
- A C compiler (Apple Clang, Clang, or GCC)
- Internet connection for the first CMake configure, which downloads Raylib 6.2

## macOS setup

Install Apple's command-line developer tools and CMake if they are not already installed:

```bash
xcode-select --install
brew install cmake
```

On most macOS setups, configure, build, and run with:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/MazeSolver
```

**SDK note for the affected macOS 27.0 Command Line Tools setup:** the linker bundled with some installations cannot parse the `arm64e.x1` architecture entry in the macOS 27 SDK. If CMake reports a TAPI error containing `unknown architecture arm64e.x1-macos`, use the installed macOS 26.5 SDK explicitly:

```bash
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DCMAKE_OSX_SYSROOT=/Library/Developer/CommandLineTools/SDKs/MacOSX26.5.sdk
cmake --build build -j4
./build/MazeSolver
```

If you previously configured the project with a different SDK, remove the generated build directory before reconfiguring:

```bash
rm -rf build
```

## Windows setup

Install CMake and a C compiler toolchain. Visual Studio Community with **Desktop development with C++** is one option. Open a Developer PowerShell or Developer Command Prompt in this folder, then run:

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

## Suggested BFS test cases

1. **Open grid:** leave all cells empty. The fixed start and destination are 28 moves apart, so BFS should report 28 moves.
2. **Detour required:** create a vertical wall in column 15, leaving only the bottom-row cell open. BFS should find the detour in 48 moves.
3. **No route:** create a complete vertical wall in column 15. BFS should report that no path exists.

These cases cover a direct shortest route, a route that must go around obstacles, and an unreachable destination.
