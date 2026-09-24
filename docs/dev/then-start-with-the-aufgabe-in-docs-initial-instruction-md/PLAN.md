# Plan — First delivery increment: Aufgabe 1a + 1b

Session: `docs/dev/then-start-with-the-aufgabe-in-docs-initial-instruction-md/`
Source of truth for requirements: `docs/initial_instruction.md`
Spec: see `SPECS.md` in this directory.

## Goal

Deliver a compilable, verified C++ program that **generates a random /
procedural 2D tile map satisfying every Aufgabe 1a and 1b requirement**,
rendered to the console, with the mandated code separation
(`*.h`/`*.cpp`, one class per header) and a verification harness that
proves the invariants.

## Environment-verified facts

- Toolchain available: g++ 11.4.0, CMake 3.22.1, Make 4.3, Ninja 1.10.1.
  No Visual Studio, no clang, no raylib. → CMake + C++17 (ADR 0001).
- Greenfield repo; issues tracked as local markdown under
  `.scratch/<feature-slug>/` (per `AGENTS.md` / `docs/agents/issue-tracker.md`).

## Key decisions (each has an ADR)

| Decision | Choice | ADR |
|---|---|---|
| Build system | CMake (≥3.16), out-of-source builds only | 0001 |
| C++ standard | C++17, no compiler extensions | 0001 |
| Map templated? | **No** — concrete `Tile` element type; cheap `Map<T>` refactor kept possible; template competence comes with `Inventory` in 2a | 0002 |
| Path guarantee | Carve a guaranteed start→exit path first (monotone-descent random walk), then randomize only the remaining cells; structural guarantee, no accept/reject loop | 0003 |
| Rendering | Console ASCII (`s`/`e`/`b`/`x`); raylib deferred behind a future `MapRenderer` interface | 0004 |
| Layout / identifiers | Flat `src/`, one class per header, English identifiers, namespace `p2game` | 0005 |

## Module breakdown (increment 1)

- `TileKind.h` — enum-only declaration: `Start`, `Exit`, `Blocked`,
  `Traversable` (permitted exception to one-class-per-header).
- `Tile` (`Tile.h`/`Tile.cpp`) — value class: kind + display glyph;
  designed so 1c can later attach item payloads without changing the API
  shape.
- `Map` (`Map.h`/`Map.cpp`) — 2D grid of `Tile`, rows/cols accessors,
  `at(row, col)`, `isTraversable(row, col)` (the traversal-query surface
  that Aufgabe 2b's `Character` interface and 4a's pathfinding will
  consume), `toString()`.
- `MapGenerator` (`MapGenerator.h`/`MapGenerator.cpp`) — automatic,
  seeded generation per ADR 0003; configurable size (min 15×15) and
  blocked probability; asserts all four kinds present.
- `MapGeneratorConfig.h` — config struct + `kMinMapRows`/`kMinMapCols`
  size bounds (helper-config exception to one-class-per-header, GR-2).
- `Demo` (`Demo.h`/`Demo.cpp`) — demo behaviour behind
  `run_demo(std::ostream&)` (a testable seam); prints 1–2 generated
  maps (one fixed seed for reproducibility, one from
  `std::random_device`) with the legend, and reports start/exit
  coordinates.
- `main.cpp` — thin wiring only: calls `run_demo` with `std::cout`
  (ADR 0004).
- `tests/` — verification harness, registered with CTest:
  `tile_test.cpp`, `map_test.cpp`, `mapgenerator_test.cpp`,
  `map_rules_test.cpp`, `demo_test.cpp`, plus the script-mode
  `docs_consistency.cmake` guard (see Verification approach below).

## Delivery sequence (for the Orchestrator's task breakdown)

1. **Scaffold build** — CMakeLists, directory layout per ADR 0005,
   empty-but-compiling targets, `.gitignore` for `build/`.
2. **Tile + TileKind** — kinds and glyphs.
3. **Map** — grid storage, accessors, `toString()`.
4. **MapGenerator** — carve-then-randomize per ADR 0003, seed support.
5. **Verification harness** — property-style tests over many seeds
   (below), wired into CTest.
6. **Demo** — demo behaviour lives in `Demo.h`/`Demo.cpp` behind
   `run_demo(std::ostream&)`; `main.cpp` stays thin (wires `run_demo`
   to `std::cout`). Final polish of output format and legend.
7. **Documentation sweep** — update the written-submission source→task
   mapping (see spec §Abgabe checklist). As delivered: the mapping doc
   lives at `docs/source-task-map.md` and the `docs_consistency` CTest
   guards doc↔code alignment.

Steps 2–3 and 4 are independently testable; 5 depends on 2–4.

## Verification approach

A test executable (`map_rules_test`) instantiates `MapGenerator` for a
sweep of seeds (e.g. 100) and sizes (15×15 minimum and a few larger),
and asserts for every generated map:

1. **Minimum size** — `rows() >= 15 && cols() >= 15` (1a b).
2. **Start/Exit position** — exactly one `Start` tile, in row 0; exactly
   one `Exit` tile, in the last row (1a d).
3. **Recognizability** — Start and Exit are distinct, dedicated kinds
   with dedicated glyphs, rendered differently from all other tiles
   (1a e).
4. **≥ 4 tile kinds** — counts of Start, Exit, Blocked, Traversable are
   each ≥ 1 (1a f); the generator pins this structurally (ADR 0003 §3).
5. **Path existence** — the harness runs its **own BFS** over
   non-blocked tiles from the Start cell to the Exit cell and requires
   success (1a c). The BFS lives in the test only, so map/generator
   routines stay free of pathfinding (assignment rule for 4a).
6. **Randomness** — different seeds produce different maps (at least:
   map for seed A ≠ map for seed B), demonstrating 1b's "at least partly
   random".

Run via `ctest` inside the build directory; all tests must pass before
the increment is considered done. A manual smoke check
(`./build/portfolio_game`) eyeballs the rendered map against the
assignment's example format.

## Risks / notes

- **WSL2 console rendering** is plain text — no platform risk.
- **raylib** is intentionally not pulled in; if the operator later wants
  the bonus 2D version, ADR 0004 names the refactor path.
- **Templating Map** stays an open option; ADR 0002 documents the cheap
  migration path if the course feedback demands it.
- **Grading rules** are enforced structurally by ADR 0005; a
  one-class-per-header check can be added to the harness later if
  desired.

## Out of scope for this increment

Aufgabe 1c (treasure tiles, `ItemBase`, items on maps), 2a/2b
(inventory, characters), 3a/3b (sorting), 4a (pathfinding demo),
5a (full game) — see spec §Extension points for how the increment-1
architecture anticipates each.
