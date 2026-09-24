### Ticket: T4c
id: T4c
title: Pathfinding — A* shortest path (Aufgabe 4a)
description: Implement `src/Pathfinding.h/.cpp` per ADR 0010: `AStarPathfinder` computing the shortest Start->Exit path with A* and the Manhattan heuristic (FR-45), plus `startOf(map)`/`exitOf(map)` helpers. Value-result API, no I/O; the module reads `Map` only through its public query surface and links `map` read-only — pathfinding is cleanly separated from the map routines (FR-47), and the generator stays a bounded random walk.
acceptance criteria:
- AStarPathfinder returns a shortest Start->Exit path; Manhattan heuristic; hand-written, no third-party libs (FR-45)
- Value-result API shape, no I/O, no Map mutation; Pathfinding has no generator/demo dependency (FR-47)
- No pathfinding logic leaks into Map/MapGenerator (FR-47)
- Tests: optimality cross-checked against a test-internal BFS oracle on generated maps (seed sweep); unreachable case on a hand-built Map (walled-off exit) reported as no-path; API shape pinned
- Full suite green
depends_on: T3d
