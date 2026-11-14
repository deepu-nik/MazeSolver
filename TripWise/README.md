# TripWise — Tourist Attraction Planner (C + Raylib)

A GUI-based C application that selects attractions maximizing total rating while satisfying both a time limit and a budget limit.

## DAA strategy
**Dynamic Programming — 0/1 Knapsack with two constraints.** The DP state is indexed by used time and used budget. Each attraction is considered once, with descending resource iteration. A brute-force subset enumeration validates small inputs (up to 22 attractions).

## Model and implementation limits
- Ratings are additive; each attraction can be selected at most once.
- Visit duration is positive; cost and rating are non-negative integers.
- Travel time, route order and opening hours are not modelled.
- Up to 40 attractions, time limit 2,000 minutes, budget limit ₹200,000.
- The DP table is capped at 2 million cells (roughly 32 MB for the current cell representation).

## Build
Requirements: C compiler (Clang on macOS; MinGW-w64/MSYS2 GCC on Windows), Raylib 5.x and Make.

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

The GUI currently uses default trip limits of 480 minutes and ₹3,000. Use the sample dataset or add attractions; the optimizer returns a best selection under both limits.
