# Specification — Remaining Aufgaben: 1c, 2a, 2b, 3a+3b, 4a, 5a

Session: complete the interrupted planning. Increment 1 (Aufgabe 1a+1b)
is delivered and green; ADRs 0006–0010 (authored by the prior session)
+ the new ADR 0011 are the architecture authority for everything below.
This spec continues the canonical FR numbering of increment 1
(FR-1..FR-12 in `docs/dev/then-start-with-the-aufgabe-in-docs-initial-instruction-md/SPECS.md`
and `docs/source-task-map.md` — do not renumber).

## 1. Purpose and scope

**In scope:** requirements + architecture + verification planning for
Aufgabe 1c (items & treasure chest), 2a (inventory & equipment slots),
2b (characters & traversal), 3a+3b (inventory sorting + demo),
4a (A* pathfinding + demo), and 5a (game integration, ADR 0011).
Everything is console-first C++17 on the existing CMake/CTest build
(ADR 0001/0004), flat `src/` layout, English identifiers (ADR 0005).

**Out of scope (non-goals):** any implementation in this session; the
optional 2D/tile-asset version (ADR 0004 keeps `MapRenderer` as a later
swap point); NPCs (ADR 0008 extension point); re-litigating ADRs
0001–0010. One recorded alignment: the Status lines of ADRs 0007–0010
said "increment 2" (the prior session's single-following-increment
label); this plan splits delivery into increments 2–5 and the Status
lines were aligned to that sequence (delivery metadata only — no
decision content changed).

## 2. Normative sources

- `docs/initial_instruction.md` — clauses 1c, 2a, 2b, 3a, 3b, 4a, 5a
  (quoted per FR below; German sub-clause letters in parentheses).
- ADRs 0006–0011 — architecture for exactly these tasks.
- Increment-1 code: `src/Map.h` (stable query surface: `rows()`,
  `cols()`, `at()`, `isTraversable()`, `toString()`), `src/MapGenerator`
  (+ `MapGeneratorConfig.h` tunables), `src/Demo.cpp`'s
  `run_demo(std::ostream&)` seam, `tests/CMakeLists.txt`'s
  guarded-library pattern, and the seed-sweep harness
  (`map_rules_test.cpp`).

## 3. Functional requirements (stable ids)

### Aufgabe 1c — items & treasure chest (increment 2)

- **FR-13** (1c 1): at least one further tile type `TreasureChest`,
  console glyph `t`, traversable like Traversable (ADR 0006) — so
  FR-3/FR-11 path guarantees stay structural.
- **FR-14** (1c 2a): class `ItemBase`, the abstract base class of all
  items.
- **FR-15** (1c 2b): `ItemBase` weight attribute; must be > 0
  (constructor-validated; violations throw `std::invalid_argument`).
- **FR-16** (1c 2c): name attribute. **FR-17** (1c 2d): description
  attribute. **FR-18** (1c 2e): value/price attribute.
- **FR-19** (1c 2f): at least one concrete item class derived from
  `ItemBase` — `GoldCoin` (ADR 0006).
- **FR-20** (1c 3a): items can lie on traversable tiles.
- **FR-21** (1c 3b): every generated map carries ≥ 5 items (visible or
  hidden — both permitted).
- **FR-22** (1c 3c): the map still satisfies everything from 1a/1b —
  FR-1..FR-12 keep holding on item maps (extended seed sweep).

### Aufgabe 2a — inventory & equipment slots (increment 3)

- **FR-23** (2a a): `Inventory` is a template-based class; the template
  parameter is forwarded to the item container (ADR 0007:
  `Inventory<ItemPtr>` with a type alias, header-only `Inventory.hpp`).
- **FR-24** (2a b): a generic (template-based) container stores the
  items; it may wrap a library container — it wraps `std::vector`.
- **FR-25** (2a c): the slot count is fixed at instantiation
  (constructor argument).
- **FR-26** (2a d): the maximum item count is never exceeded (overflow
  refused, state unchanged).
- **FR-27** (2a e): set/get methods, at least one where the slot index
  must be given (`setSlot`/`getSlot`/`removeSlot`).
- **FR-28** (2a eq a,b): 3 dedicated equipment slots, each accepting
  exactly one specific item class — slot 1 `Ring`, slot 2 `Trousers`,
  slot 3 `Weapon`; all three are concrete `ItemBase` children extending
  the 1c hierarchy (ADR 0006/0007).
- **FR-29** (2a eq c): the equipment classes carry a "+strength"
  attribute (`strengthBonus()`); the optional extra attribute is
  realized as `Weapon`'s "+x% damage" bonus.

### Aufgabe 2b — characters & traversal (increment 3)

- **FR-30** (2b 1a): class `Character` — base class for PC and NPC —
  defines an interface enabling map traversal (ADR 0008: `canEnter`,
  `moveTo`, `glyph()`).
- **FR-31** (2b 2): `PlayerChar` can move on the map (validated moves
  via the `Character` interface; out-of-bounds / Blocked refused).
- **FR-32** (2b 3,4): `PlayerChar` owns an `Inventory` with 10 item
  slots plus the equipment slots.
- **FR-33** (2b 5,6): `PlayerChar` has a strength attribute; strength
  limits the maximum carryable weight: `maxWeight() = strength() × 2`
  kg (10 strength → 20 kg, the assignment's example; ADR 0008 constant
  `kKgPerStrength`).
- **FR-34** (2b 7): while the carry weight is *exceeded*, the figure
  cannot move on the map (`moveTo` refuses; at exactly the limit
  movement is still allowed).
- **FR-35** (2b 8): interface for picking up and dropping items
  (`pickUpFrom`, `dropTo`).
- **FR-36** (2b 9): interface for equipping items into the equipment
  slots (`equip`; the displaced item returns to a free item slot).
- **FR-37** (2b 10): equipping "+strength" items adjusts the figure's
  strength (strength = base + equipped bonuses; ADR 0008 computes it,
  never stores a stale copy).

### Aufgabe 3a — inventory sorting (increment 4)

- **FR-38** (3a a): sorting by weight, ascending and descending.
- **FR-39** (3a b): sorting by name, alphabetically ascending and
  descending.
- **FR-40** (3a c): sorting by value/price, ascending and descending.
  All six combinations via hand-written stable merge sort — no
  `std::sort`/`std::stable_sort` in the graded path (ADR 0009).

### Aufgabe 3b — sorting demo (increment 4)

- **FR-41** (3b 1): all 3a sort functions are demonstrated.
- **FR-42** (3b 2): the demonstrated inventories hold 10–20 items.
- **FR-43** (3b 3): items are chosen so correct sorting is recognizable
  (pairwise-distinct weights / names / values; seeded
  `ItemFactory` + a fixed catalog).
- **FR-44** (3b 4): the user recognizes what is demonstrated: a header
  per demonstration, and the unsorted AND sorted inventory shown
  (`run_sorting_demo(std::ostream&)` console output).

### Aufgabe 4a — pathfinding (increment 4)

- **FR-45** (4a a): the shortest path from Start to Exit is determined
  by a shortest-path algorithm — A* with Manhattan heuristic
  (ADR 0010).
- **FR-46** (4a): a demo program computes the path for a 1b-generated
  map and outputs the result to the user: path length, the coordinates
  walked, and the map with the path overlaid as `*`
  (`run_pathfinding_demo(std::ostream&)`).
- **FR-47** (4a separation rule): pathfinding routines are cleanly
  separated from the map routines and classes — own module
  `Pathfinding.h/.cpp`, only reads `Map` via the public query surface,
  value-result API, no I/O; the generator stays a bounded random walk,
  never a search (per increment-1 `MapGenerator.h` comment).

### Aufgabe 5a — game integration (increment 5; ADR 0011)

- **FR-48** (5a intro): the game honors ALL constraints from Aufgaben
  1–4 (traversable tiles, weight limit, inventory/item limits).
- **FR-49** (5a 1,2): a program spawns a `PlayerChar` on the Start tile
  of a randomly/procedurally generated map (requirements identical to
  1b).
- **FR-50** (5a 3): every map contains ≥ 3 distinct items, ≥ 1
  equipable — structurally guaranteed by ADR 0006's per-class top-up
  (≥ 1 GoldCoin/Ring/Trousers/Weapon each).
- **FR-51** (5a 4): the game is turn-based (ADR 0011).
- **FR-52** (5a 5): the user can traverse the map manually.
- **FR-53** (5a 6): the user can have the map traversed automatically
  by the pathfinding algorithm.
- **FR-54** (5a 7): the goal is reaching the Exit.
- **FR-55** (5a 8): the user can have the inventory sorted in ≥ 2 ways
  (the six 3a combinations are offered).
- **FR-56** (5a manual 1): the player figure is visualized on the map
  (`@` overlay via `ConsoleRenderer`).
- **FR-57** (5a manual 2): entering a tile with an item offers pick-up
  and/or equip.
- **FR-58** (5a manual 3): the user has access to the inventory at any
  time.
- **FR-59** (5a auto 1): the automatically traversed path is
  visualized (`*` overlay).
- **FR-60** (5a auto 2 a+b): items on automatically traversed tiles are
  auto-picked-up while (a) a free item slot exists and (b) the pick-up
  would not exceed `maxWeight()` — otherwise skipped, never taken.
- **FR-61** (5a auto 3): an item whose `strengthBonus()` is better than
  the currently equipped item's (for that slot type) is auto-equipped;
  if the swap is impossible (no free item slot for the displaced item),
  auto-equip is skipped (ADR 0011 policy).
- **FR-62** (5a auto 4): at the end the user can display and sort the
  inventory.

## 4. Grading constraints (inherited, unchanged)

GR-1..GR-5 from increment 1 still govern: code separation (`*.h`/`*.cpp`,
templates in `*.hpp`), one class per header (enum-only and
helper-config exceptions), consistent English identifiers, no
third-party libraries (A*, merge sort, tests all hand-written), CTest
verification, out-of-source builds excluded from the Abgabe archive.

## 5. Architecture and module breakdown (ADRs 0006–0011)

Flat `src/` (ADR 0005). Boundaries: `Map` stays a pure data +
query structure; the generator generates; `Pathfinding` only reads
`Map`; visualization lives in renderers; the game orchestrates.

**Increment 2 (1c):** `TileKind.h` +`TreasureChest`; `Tile.h/.cpp`
+`item()`/`setItem()` non-owning view; `ItemBase.h/.cpp` (abstract);
`GoldCoin.h/.cpp`; `ItemFactory.h/.cpp` (seeded catalog);
`Map.h/.cpp` +item ownership (`std::vector<std::unique_ptr<ItemBase>>`,
`itemAt()`); `MapGeneratorConfig.h` +item tunables;
`MapGenerator.cpp` +placement step with deterministic per-class top-up;
`Demo.cpp` legend gains `t`.

**Increment 3 (2a+2b):** `Ring.h/.cpp`, `Trousers.h/.cpp`,
`Weapon.h/.cpp` (equipment items, `strengthBonus()`, Weapon
+damage%); `Inventory.hpp` (class template, vector-backed, typed
equipment members); `Character.h/.cpp` (abstract traversal interface);
`PlayerChar.h/.cpp` (10 slots, computed strength, weight rule,
pick/drop/equip).

**Increment 4 (3a+3b + 4a):** `SortKey.h` (enum-only);
`ItemSorters.hpp` (`mergeSort(It, It, Less)` + `sortInventory(...)`);
`SortingDemo.h/.cpp`; `Pathfinding.h/.cpp` (`AStarPathfinder`,
`startOf(map)`/`exitOf(map)`); `PathfindingDemo.h/.cpp`; thin mains
add the `sorting_demo` and `pathfinding_demo` executables (same pattern
as `src/main.cpp` → `portfolio_game`).

**Increment 5 (5a):** `ConsoleRenderer.h/.cpp` (map + `@`/`*`/`t`
overlays, precedence `@ > * > tile glyph`); `GameConfig.h` (helper
struct: seed, `MapGeneratorConfig`, base strength);
`Game.h/.cpp` (`run(std::istream&, std::ostream&)`, turn loop, manual
commands, automated mode, end-of-game sort); thin main → executable
`game`. Existing demos stay untouched.

## 6. Verification approach

CTest only (step `test`: `ctest --output-on-failure`), hand-rolled
assert-style tests (GR-4), extending increment-1 philosophy:

- **Guarded-library pattern** (`tests/CMakeLists.txt` `if(EXISTS ...)`)
  keeps the tree configurable before each increment's sources land; new
  tests register beside the existing ones.
- **Seed-sweep extension**: `map_rules_test`-style sweeps assert, for
  ~100 seeds × sizes: FR-21 (≥ 5 items), ADR 0006 top-up (≥ 1 item per
  class ⇒ FR-50), chest traversability + ≥ 1 chest, and ALL FR-1..FR-12
  invariants on item maps (FR-22).
- **Independent oracles, test-only**: the test-internal BFS stays the
  oracle for path existence (increment 1) and shortest length
  (pathfinding optimality cross-check, FR-45); sorting tests may
  cross-check `mergeSort` against `std::stable_sort` in the TEST file
  only (ADR 0009 — the graded path stays hand-written).
- **Game sessions** are deterministic functions of (GameConfig, seed,
  input script): scripted `std::istream` sessions cover manual walk to
  exit (FR-52/54), weight-blocked movement (FR-34), item-tile offer
  (FR-57), auto pick-up/equip policies (FR-60/61), path overlay
  (FR-59), and end-of-game sort (FR-62).
- **Negative cases**: unreachable goal via a hand-built `Map` (walls
  around the exit) — constructed directly, not via the generator;
  weight ≤ 0 rejected (FR-15); inventory overflow refused (FR-26).
- **Docs guard**: each increment extends `tests/docs_consistency.cmake`'s
  `shipped_files` list and `docs/source-task-map.md` (see §7), so the
  mapping keeps naming every shipped file and the suite stays green.

## 7. Source→task mapping update (lands incrementally in `docs/source-task-map.md`)

Rows to add when the respective increment lands (the existing FR-1..12
rows stay untouched):

| File | Task clause(s) | Requirement(s) |
| --- | --- | --- |
| src/TileKind.h, src/Tile.h, src/Tile.cpp | 1c 1, 5a | FR-13 (+chest kind; item view on Tile) |
| src/ItemBase.h, src/ItemBase.cpp | 1c 2a–e | FR-14..FR-18 |
| src/GoldCoin.h, src/GoldCoin.cpp | 1c 2f | FR-19 |
| src/ItemFactory.h, src/ItemFactory.cpp | 1c 3a,b | FR-20, FR-21, FR-43 |
| src/Map.h, src/Map.cpp | 1c 3a–c | FR-20..FR-22 (item ownership, `itemAt`) |
| src/MapGenerator.h/.cpp, src/MapGeneratorConfig.h | 1c 3b, 5a 3 | FR-21, FR-50 (placement + top-up) |
| src/Demo.cpp (legend) | 1c 1 | FR-13 (glyph `t` in the legend) |
| src/Inventory.hpp | 2a a–e | FR-23..FR-27, FR-28 (slots), FR-29 |
| src/Ring.h/.cpp, src/Trousers.h/.cpp, src/Weapon.h/.cpp | 2a eq a–c | FR-28, FR-29 |
| src/Character.h, src/Character.cpp | 2b 1a | FR-30 |
| src/PlayerChar.h, src/PlayerChar.cpp | 2b 2–10 | FR-31..FR-37 |
| src/SortKey.h | 3a | FR-38..FR-40 (enum-only exception) |
| src/ItemSorters.hpp | 3a | FR-38..FR-40 |
| src/SortingDemo.h/.cpp (+ its thin main) | 3b 1–4 | FR-41..FR-44 |
| src/Pathfinding.h, src/Pathfinding.cpp | 4a a | FR-45, FR-47 |
| src/PathfindingDemo.h/.cpp (+ its thin main) | 4a | FR-46 |
| src/GameConfig.h, src/Game.h, src/Game.cpp (+ its thin main) | 5a | FR-48..FR-55, FR-62 |
| src/ConsoleRenderer.h, src/ConsoleRenderer.cpp | 5a manual 1, auto 1 | FR-56, FR-59 |
| new test files (item/…/game tests) | — | GR-5: verification harness, shipped as tests |

## 8. Risks and notes

- The 4a separation rule is the sharpest grading edge: FR-47 and the
  ADR 0010 boundary must hold in every increment (the sweep's BFS and
  any oracle stay in test files).
- `Inventory.hpp` must compile self-contained (templates ship in
  headers, GR-1); instantiate it with at least two pointer types in
  tests to prove genericity (FR-23).
- The 2b weight rule uses *exceeded*, not *reached*: tests must pin
  the boundary case (weight == maxWeight still moves).
- 5a auto-pick-up must never over-weight (FR-60) while manual pick-up
  may (then movement is blocked, FR-34) — the two modes differ; tests
  pin both.
