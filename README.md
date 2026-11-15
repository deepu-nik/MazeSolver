# DAA Mini Projects — SY BTech CSE, AY 2026–27

Two real-world GUI applications implemented in **C11 with Raylib 5.x**.

- [MessMate](MessMate/README.md) — college mess scheduler using maximum bipartite matching.
- [TripWise](TripWise/README.md) — tourist attraction planner using two-constraint 0/1 Knapsack Dynamic Programming.

## Requirements
C compiler (Clang on macOS; MinGW-w64/MSYS2 GCC on Windows), Raylib 5.x and Make. No Python runtime is required.

## Clone
```bash
git clone -b feat/messmate-tripwise-gui https://github.com/deepu-nik/MazeSolver.git
cd MazeSolver
```
See each project README for installation and build steps.

## Tests
Algorithm tests do not require Raylib:
```bash
cd MessMate && make test
cd ../TripWise && make test
```

## Collaboration
Both teammates clone the same branch and work on separate feature branches. Pull latest changes before starting and use pull requests. Avoid editing the same file simultaneously.

## Submission checklist
Fill team names, PRNs and guide details; compile and run the GUI on the demonstration computers; capture real screenshots; retain actual compiler/test output; verify reports against the final implementation.
