# Specification — Portfolio increment 1: Aufgabe 1a + 1b (random map generation)

Project: Hochschule C++ "Programmieren 2" Portfolio (greenfield)
Source of truth: `docs/initial_instruction.md` (assignment) — clauses are quoted/referenced below.
Scope decision (Orchestrator): increment 1 = **Aufgabe 1a + 1b combined** (assignment: "Aufgabe 1a und 1b dürfen zusammen in einer Aufgabe bearbeitet werden!").
Glossary: `CONTEXT.md` (Map, Tile kinds Start/Exit/Blocked/Traversable, Path, Portfolio task, Abgabe).

## 1. Overview

The program automatically generates a 2D map of tiles (minimum 15×15)
that always contains a traversable path from a recognizable Start tile
in the first row to a recognizable Exit tile in the last row, uses at
least 4 tile kinds, and is at least partly random/procedural. The map
is rendered to the console. The implementation follows the assignment's
mandatory code-separation rules.

## 2. In scope / out of scope

**In scope:** everything needed to satisfy Aufgabe 1a + 1b: tile kinds,
Map class, automatic seeded generation, console rendering, demo `main`,
build system, verification harness.

**Out of scope (this increment):** Aufgabe 1c (treasure tiles/`ItemBase`/
items), 2a (templated `Inventory`), 2b (`Character` classes), 3a/3b
(sorting), 4a (pathfinding as a demoed algorithm), 5a (interactive game
traversal). The Abgabe packaging itself (archive, written submission) is
the operator's submission-time concern — recorded in §7, not built now.

## 3. Functional requirements

Numbering keeps a stable id per assignment clause.

- **FR-1 (1a a, f)** The program represents a 2D map of tiles; each tile
  has one of at least four kinds: `Start`, `Exit`, `Blocked`,
  `Traversable`. Displayed glyphs follow the assignment legend:
  `s`, `e`, `b`, `x`.
- **FR-2 (1a b)** Minimum map size 15×15 tiles. The generator supports
  larger maps (configurable width/height ≥ 15).
- **FR-3 (1a c)** Every generated map contains a traversable path from
  start to exit.
- **FR-4 (1a d)** Start is in the first row, Exit is in the last row.
- **FR-5 (1a e)** Start and Exit are recognizable: dedicated kinds and
  glyphs, distinct from every other tile.
- **FR-6 (1a g)** The map is implemented as a class (`Map`), not bare
  arrays/functions.
- **FR-7 (1a h)** `Map` is not templated (decision, see ADR 0002) — the
  assignment's optional template variant is consciously declined, with
  the migration path documented.
- **FR-8 (1a i)** Rendering is console ASCII (decision, see ADR 0004);
  raylib is not used in this increment.
- **FR-9 (1b a)** Map generation is automatic — a plain program run
  produces a complete map with no hand-made input.
- **FR-10 (1b b)** Generation is at least partly random: start/exit
  columns, path shape, and off-path blocked/traversable mix are
  randomized per seed (`<random>`, seeded `std::mt19937`).
- **FR-11** Generation guarantees FR-2..FR-5 *structurally*
  (carve-then-randomize, ADR 0003), not by accept/reject loops.
- **FR-12** The generator is seed-parameterized; default seeding uses
  `std::random_device`; a fixed seed yields a reproducible map.

## 4. Non-functional / grading requirements (hard constraints from the assignment)

- **GR-1** Mandatory code separation and structuring: declarations in
  `*.h`, definitions in `*.cpp`; templates in `*.h`/`*.hpp` only.
- **GR-2** One class per header; permitted exceptions: helper classes
  and enum-only type declarations (`TileKind.h` uses the enum
  exception). Violations cost points.
- **GR-3** Consistent identifiers: English throughout (ADR 0005),
  single project namespace `p2game`.
- **GR-4** Third-party libraries only where the task permits; any use
  must reference sources. Increment 1 uses none.
- **GR-5** A Visual Studio Solution **or** CMake project for the
  portfolio solution → CMake chosen (ADR 0001).

## 5. Architecture / module breakdown

```
CMakeLists.txt
src/
  TileKind.h                        // enum TileKind { Start, Exit, Blocked, Traversable }
  Tile.h        Tile.cpp            // class Tile: kind + glyph (value type)
  Map.h         Map.cpp             // class Map: grid, rows()/cols(), at(), isTraversable(), toString()
  MapGenerator.h MapGenerator.cpp   // class MapGenerator: carve-then-randomize, seed, tunables
  MapGeneratorConfig.h              // struct MapGeneratorConfig + kMin* size bounds
                                    //   (helper-config exception to one-class-per-header, GR-2)
  Demo.h        Demo.cpp            // demo behaviour behind run_demo(std::ostream&) (testable seam)
  main.cpp                          // thin wiring: run_demo -> std::cout (ADR 0004)
tests/
  tile_test.cpp   map_test.cpp      // unit tests (CTest)
  mapgenerator_test.cpp             // generator-level tests (CTest)
  map_rules_test.cpp                // map-invariant sweep over seeds/sizes (CTest)
  demo_test.cpp                     // demo output tests (CTest)
  docs_consistency.cmake            // doc-code alignment guard (script-mode CTest)
```

- **Map** is data + traversal queries: `at(row, col)`,
  `isTraversable(row, col)`, `rows()`, `cols()`, `toString()`. It holds
  no pathfinding, no generation logic, no I/O beyond `toString()`.
- **MapGenerator** owns generation and its randomness. Algorithm per
  ADR 0003: (1) random start column in row 0, random exit column in the
  last row; (2) monotone-descent random walk (down preferred, bounded
  horizontal runs, no up-moves) carving Traversable cells — a valid
  start→exit path by construction; (3) fill remaining cells randomly
  (Blocked ≈30%, else Traversable); (4) pin a minimum number of Blocked
  tiles so all four kinds are always present. The tunables live in
  `MapGeneratorConfig.h` as a configuration surface per §8:
  `rows`/`cols` default 15x15 with `kMinMapRows = kMinMapCols = 15`
  (`std::invalid_argument` is thrown below the minimum, FR-2),
  `p_blocked = 0.3`, `horizontal_step_cap = 3`, `min_blocked = 1`.
- **Tile** is a small value class (kind + glyph) so 1c can later attach
  item payloads without reworking the Map API.
- Demo behaviour lives in `Demo.h`/`Demo.cpp` behind
  `run_demo(std::ostream&)` (a testable seam); `src/main.cpp` stays thin
  and only wires `run_demo` to `std::cout` (ADR 0004). `run_demo` builds
  one fixed-seed map and one random-device-seeded map, prints each with
  the legend (`s=Start, e=Exit, b=Blocked, x=Traversable`) and
  start/exit coordinates; exit code 0.

## 6. Verification approach

CTest suite as implemented: `tile_test`, `map_test`,
`mapgenerator_test`, `map_rules_test`, `demo_test`, plus the
script-mode `docs_consistency` check (no compiler needed). The
`map_rules_test` property sweep; for a sweep of
seeds (e.g. 100) and sizes (15×15 plus a few larger), assert:

1. `rows() >= 15 && cols() >= 15` (FR-2).
2. Exactly one `Start`, in row 0; exactly one `Exit`, in the last row
   (FR-4).
3. Start/Exit kinds and glyphs are distinct from all others (FR-5).
4. Counts of all four kinds ≥ 1 (FR-1).
5. Harness-internal **BFS** over non-blocked tiles finds a Start→Exit
   path (FR-3). BFS lives only in the test — map/generator code stays
   pathfinding-free.
6. Seed A and seed B (same dimensions) produce different maps (FR-10).

Plus a manual smoke check: `portfolio_game` renders a plausible map.

The `docs_consistency` check guards doc↔code alignment: the source→task
mapping doc (`docs/source-task-map.md`) exists and names every shipped
file, FR-9 (automatic generation) is never conflated with min(imum)
(FR-2 owns the 15x15 minimum-size wording), and every SPECS.md/PLAN.md
mentions `MapGeneratorConfig.h` and `Demo.cpp`.

**Done when:** CTest suite green; demo runs; sources honor GR-1/GR-2;
increment docs (this spec + plan) match the code.

## 7. Abgabe checklist (operator's concern — recorded, not built now)

- Zip/rar/7z archive for Moodle upload; **no** build/Debug/Release
  directories inside.
- Written submission (pdf/word) listing which sources implement which
  task — maintain a source→task mapping as code lands:
  `src/TileKind.h`, `src/Tile.*`, `src/Map.*`, `src/MapGenerator.*`,
  `src/main.cpp` → Aufgabe 1a + 1b.
- Optional separate folder with Windows binaries + a read-me
  (raylib optional). Console version is the baseline.

## 8. Extension points (later aufgaben must not be foreclosed)

- **1c** — Treasure tiles/items: `TileKind` enum extends cleanly;
  `Tile` is designed to carry a future item payload (pointer/reference
  to an `ItemBase`); placement becomes an extra `MapGenerator` step;
  `ItemBase` gets its own header (one-class-per-header).
- **2a** — Templated `Inventory`: independent module; demonstrates
  templates (`*.hpp`) required by the assignment; no impact on `Map`.
- **2b** — `Character` base class defines "ein Interface, das eine
  Traversierung der map ermöglicht": `Map::isTraversable(row, col)` +
  size accessors already are that surface; characters consume `Map`,
  never modify its generation logic.
- **3a/3b** — Sorting: operates on the inventory/item domain (separate
  module), no Map impact.
- **4a** — Pathfinding routines must be cleanly separated from map
  routines/classes: they live in their own module (e.g.
  `src/pathfinding/`) and consume only `Map`'s traversal queries.
  The generator's internal walk is *not* a pathfinding algorithm.
- **5a** — Interactive traversal: `Map` gains no mutable generation
  state; traversal state (character position) lives in the character
  module.
- **Bonus areas** — 2D/raylib behind a `MapRenderer` interface (ADR
  0004); generator tunables (size, `p_blocked`, walk shape) already
  form a configuration surface ("Mehr Konfigurationsmöglichkeiten").

## 9. Open questions

None blocking. Two recorded options (not blockers):
- If course feedback later favors the optional templated `Map`, migrate
  per ADR 0002's mechanical path.
- raylib stays out until the operator asks for the 2D bonus (ADR 0004).

## 10. Decisions referenced

- ADR 0001 — CMake build system, C++17
- ADR 0002 — Map is not templated in increment 1
- ADR 0003 — Carve guaranteed path first, then randomize
- ADR 0004 — Console rendering first, raylib deferred
- ADR 0005 — Flat src/ layout, English identifiers, one class per header
