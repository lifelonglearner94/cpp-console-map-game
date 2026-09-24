# Plan — Remaining Aufgaben delivery (increments 2–5)

Builds on increment 1 (Aufgabe 1a+1b, delivered green: Map/Tile/TileKind,
MapGenerator + MapGeneratorConfig.h, Demo.cpp + run_demo seam, the
guarded-library test wiring and the seed sweep). Requirements: see this
session's SPECS.md (FR-13..FR-62). Architecture: ADRs 0006–0011 —
this plan changes none of them; it sequences their delivery.

## Delivery model

Four sequential increments, each a vertical slice that lands GREEN on
the full CTest suite (step `test`: `ctest --output-on-failure`) and
extends `docs/source-task-map.md` + `tests/docs_consistency.cmake`'s
`shipped_files` list as part of the same slice. The guarded-library
pattern (`tests/CMakeLists.txt`) means the tree stays configurable at
every intermediate commit, so a slice may itself split into smaller
tickets (e.g. module + tests per module) without ever being red.

## Increment 2 — Aufgabe 1c (items & treasure chest)

Scope: FR-13..FR-22. Modules: `TileKind.h` (+`TreasureChest`),
`Tile.h/.cpp` (+item view), `ItemBase.h/.cpp`, `GoldCoin.h/.cpp`,
`ItemFactory.h/.cpp`, `Map` item ownership + `itemAt()`,
`MapGeneratorConfig.h` item tunables, `MapGenerator.cpp` placement +
per-class top-up, `Demo.cpp` legend `t` (ADR 0006).
Tests: item unit tests (weight > 0 throws, attribute accessors,
`describe()`), factory determinism, extended seed sweep — every
FR-1..FR-12 invariant plus ≥ 5 items (FR-21), ≥ 1 item per class,
chests traversable, ≥ 1 chest per map.
Verify: green suite; `docs/source-task-map.md` + docs_consistency rows
for the new files.

## Increment 3 — Aufgabe 2a + 2b (inventory, equipment, characters)

Scope: FR-23..FR-37. Modules: `Ring.h/.cpp`, `Trousers.h/.cpp`,
`Weapon.h/.cpp` (strengthBonus, Weapon +damage%), `Inventory.hpp`
(template, vector-backed, fixed slots, typed equipment members —
ADR 0007), `Character.h/.cpp` (abstract traversal interface —
ADR 0008), `PlayerChar.h/.cpp` (10 slots, computed strength,
`maxWeight() = strength() × kKgPerStrength`, pick/drop/equip).
Tests: inventory (two pointer-type instantiations prove genericity,
overflow refused, set/get-by-index, equipment slot type rules),
character traversal (bounds/blocked/exit semantics), PlayerChar weight
rule (over-weight blocks movement, at-limit still moves — the
boundary case), equip swap returns displaced item to a free slot,
strength recomputes from equipped bonuses.
Verify: green suite; mapping/docs rows.

## Increment 4 — Aufgabe 3a + 3b + 4a (sorting, demos, A*)

Scope: FR-38..FR-47. Modules: `SortKey.h`, `ItemSorters.hpp`
(hand-written stable merge sort + `sortInventory` — ADR 0009),
`SortingDemo.h/.cpp` (+`sorting_demo` executable, 10–20 pairwise
distinct items per demonstration), `Pathfinding.h/.cpp`
(`AStarPathfinder`, `startOf`/`exitOf` — ADR 0010),
`PathfindingDemo.h/.cpp` (+`pathfinding_demo` executable, `*` overlay).
Tests: sorting — six combinations cross-checked against
`std::stable_sort` as a TEST-ONLY oracle (empty/one/full inventories);
pathfinding — optimality vs test-internal BFS on generated maps, seed
sweep, unreachable case on a hand-built Map, value-result API shape,
`Pathfinding` links `map` read-only (no generator/demo dependency).
Verify: green suite; mapping/docs rows; both demo executables run.

## Increment 5 — Aufgabe 5a (game integration — ADR 0011)

Scope: FR-48..FR-62. Modules: `ConsoleRenderer.h/.cpp` (`@`/`*`/`t`
overlays, precedence `@ > * > tile glyph`), `GameConfig.h`,
`Game.h/.cpp` (`run(std::istream&, std::ostream&)`, turn loop, manual
mode, automated mode, end-of-game sort), thin main → `game`
executable. Existing demos untouched.
Tests: scripted `std::istream` sessions — manual walk to exit;
weight-blocked movement after an over-weight manual pick-up; item-tile
offer (pick/equip/ignore); auto-traversal: path overlay, auto-pick-up
with the two skip conditions (no free slot / would over-weight),
auto-equip better bonus + skip when swap impossible; end-of-game
inventory display + sort; seed sweep for FR-50; the game never mutates
`Pathfinding`/`Map` internals.
Verify: green suite; mapping/docs rows; `game` runs interactively.

## Sequencing rationale

1c before 2a/2b: the item hierarchy and Map-owned storage are the
foundation the inventory and characters consume. 2a+2b together: the
equipment classes complete the 1c hierarchy while the inventory that
stores them lands. 3a/3b + 4a together: two independent leaf modules on
the finished inventory/map substrate. 5a last: integrates everything,
purely additive (ADRs 0006–0010 modules are never reopened — the only
cross-increment touch points are additive extensions listed in the
SPECS §5 module table).

## Risks

- 4a separation rule (FR-47): pathfinding logic must never leak into
  Map/generator; oracles live in test files only.
- Template header hygiene: `Inventory.hpp`/`ItemSorters.hpp` must
  compile self-contained (GR-1) — covered by including them first in
  their tests.
- 5a policy edges: auto-pick-up may never over-weight (FR-60) while
  manual pick-up may (FR-34 then blocks movement) — both pinned by
  tests.
- Every increment must extend the docs guard with its shipped files;
  forgetting keeps the suite green but the Abgabe mapping incomplete
  (the written submission names files per task).
