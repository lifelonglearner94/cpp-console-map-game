# 0010 — Pathfinding: self-contained A* (over Dijkstra), query-only consumption of Map

## Status

Accepted (increment 4: Aufgabe 4a)

## Context

Aufgabe 4a: implement **or integrate** A* *or* Dijkstra for the
shortest path from the start position to the exit position on a 1b map,
expose a demo program that computes and displays the shortest path for
a 1b-generated map, and keep the pathfinding routines cleanly separated
from the map routines/classes. Existing libraries are allowed.

## Decision

- **Algorithm: A\*.** Reasoning: the map is a unit-cost, 4-connected
  grid, so A* with the admissible Manhattan heuristic and Dijkstra are
  *both* guaranteed optimal here — A* is exactly Dijkstra guided by the
  heuristic term, i.e. same shortest-path guarantee with strictly fewer
  expansions as the map config grows beyond 15×15; Dijkstra adds nothing
  on this domain, and A* generalizes cleanly (the heuristic is the one
  place that would change if tile costs were ever weighted). The
  implementation is `std::priority_queue`-based (C++17 std only).
- **Self-contained in `src/Pathfinding.h/.cpp`** (class
  `AStarPathfinder`, plus `startOf`/`exitOf` locating helpers) rather
  than a third-party library: keeps the Abgabe archive dependency-free
  (GR-4 hygiene: sources + CMake only) and makes the separation rule
  *visible* — pathfinding lives in its own one-class-per-header module.
- **Separation mechanics (the graded rule):** `AStarPathfinder` takes
  `const Map&` and consumes **only the read query surface**
  (`rows()`, `cols()`, `isTraversable(row, col)`, `at(...).kind()`);
  it never writes to `Map`, contains no I/O, and returns a value
  result (`std::vector<std::pair<std::size_t, std::size_t>> path`,
  empty ⇔ unreachable). `Map`, `MapGenerator`, and `Tile` remain
  free of any pathfinding code (the increment-1 test's BFS stays
  test-only and now serves as an independent optimality cross-check).
- **`PathfindingDemo` (`PathfindingDemo.h/.cpp`)** behind
  `run_pathfinding_demo(std::ostream&)`: generates a seeded 1b map,
  computes the path, and prints the map with a `'*'` path overlay plus
  the path length and start/exit coordinates.

## Rationale

"Implement" (vs. integrate) keeps the Abgabe self-contained and lets
the written submission reference the exact source lines, as the
assignment demands for every used source.

## Consequences / extension point

- 5a's automated traversal consumes the same `AStarPathfinder` (ADR 0011);
  the path visualization is drawn by the renderer, not by the pathfinder
  (separation preserved end-to-end).
- If tile weights are ever introduced (bonus idea), only the priority
  ordering and the heuristic change inside this one module.
