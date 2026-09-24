### Ticket: T4d
id: T4d
title: PathfindingDemo executable (Aufgabe 4a)
description: Implement `src/PathfindingDemo.h/.cpp` with a `run_pathfinding_demo(std::ostream&)` seam plus a thin main adding `pathfinding_demo` (FR-46): generate a 1b map, compute the path with AStarPathfinder, and output the path length, the coordinates walked, and the map with the path overlaid as `*`. The demo contains no pathfinding logic of its own — it only orchestrates generator + pathfinder + output (FR-47).
acceptance criteria:
- pathfinding_demo builds and runs
- Output shows path length, walked coordinates, and the map with `*` path overlay (FR-46)
- Demo delegates all pathfinding to Pathfinding (FR-47); deterministic given the seed
- Existing demos untouched; full suite green
depends_on: T4c
