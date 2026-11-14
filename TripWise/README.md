# TripWise — Tourist Attraction Planner (C + Raylib)

A GUI-based C application that selects attractions maximizing total rating while satisfying both a time limit and a budget limit.

## DAA strategy
**Dynamic Programming — 0/1 Knapsack with two constraints.** The DP state is indexed by used time and used budget. Each attraction is considered once, with descending resource iteration. Brute-force subset enumeration validates small inputs (up to 22 attractions).

The implementation supports up to 40 attractions, time limits up to 2,000 minutes, budget limits up to ₹200,000, and caps the DP table at 12 million cells. Ratings are additive; attractions are independent. Travel time, route order and opening hours are not modelled.

## Build
Requirements: C compiler (Clang on macOS; MinGW-w64/MSYS2 GCC on Windows), Raylib 5.x, and Make.

macOS: `brew install raylib pkg-config`
Windows: install Raylib and configure compiler include/library paths.

```bash
make
./tripwise
``
Windows: `mingw32-make` then `tripwise.exe`.

Run algorithm tests without Raylib:
```bash
make test
``
