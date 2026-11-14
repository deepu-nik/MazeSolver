# MessMate — College Mess Meal Scheduler (C + Raylib)

A GUI-based C application that assigns students to fixed service slots and maximizes the number served within their personal availability windows.

## DAA strategy
**Graph Algorithms — Maximum Bipartite Matching.** Students form the left vertex set and feasible counter time slots form the right vertex set. An edge exists only if the complete service interval fits inside the student's availability window. An augmenting-path DFS computes a maximum-cardinality matching.

## Model assumptions
- One serving counter.
- All students require a common fixed service duration of 10 minutes.
- Service starts are considered every 5 minutes between 11:30 and 14:00.
- A complete 10-minute service interval must fit inside the student's availability window.
- The algorithm is exact for this discretized model, not arbitrary continuous-time scheduling or varying service durations.
- Implementation limits: 100 students and 200 service slots.

## Build
Requirements: C compiler (Clang on macOS; MinGW-w64/MSYS2 GCC on Windows), Raylib 5.x and Make.

macOS: `brew install raylib pkg-config`
Windows: install Raylib and configure compiler include/library paths.

```bash
make
./messmate
``
Windows: `mingw32-make` then `messmate.exe`.

Run algorithm tests without Raylib:
```bash
make test
``

The GUI supports adding a student, loading sample records, removing the last record, optimizing and clearing the list. Click a text field to type; TAB changes the active field.
