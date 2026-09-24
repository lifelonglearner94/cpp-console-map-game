# Source-to-Task Mapping

Maintained mapping of every shipped source file to the assignment task
clauses and functional requirements (FR ids, spec §3), per the
assignment's Abgabe requirement (the written submission must list which
sources implement which task) and the project spec §7. The failing-first
CTest `docs_consistency` (script `tests/docs_consistency.cmake`) guards
this file: it must name every shipped file and record the archive
hygiene rule below.

Canonical FR numbering (spec §3, do not renumber):

- FR-1 four kinds + glyphs — Aufgabe 1a a,f
- FR-2 minimum 15x15, configurable larger — Aufgabe 1a b
- FR-3 path — Aufgabe 1a c
- FR-4 Start row 0 / Exit last row — Aufgabe 1a d
- FR-5 recognizable Start/Exit — Aufgabe 1a e
- FR-6 Map is a class — Aufgabe 1a g
- FR-7 Map not templated — Aufgabe 1a h (ADR 0002)
- FR-8 console rendering — Aufgabe 1a i (ADR 0004)
- FR-9 automatic generation — Aufgabe 1b 1
- FR-10 partly random — Aufgabe 1b 2
- FR-11 structural guarantee — ADR 0003
- FR-12 seed-parameterized

## Mapping

| File | Task clause(s) | Requirement(s) / notes |
| --- | --- | --- |
| src/TileKind.h | 1a a,f | FR-1 (the four kinds; enum-only exception to one-class-per-header, GR-2) |
| src/Tile.h | 1a a,f | FR-1, FR-5: kind + glyph value type |
| src/Tile.cpp | 1a a,f | FR-1, FR-5: kind + glyph value type |
| src/Map.h | 1a b,f | FR-2 (stores any grid >= 15x15), FR-6 (a class, not arrays), FR-7 (not templated, ADR 0002); toString() serves 1a i / FR-8 |
| src/Map.cpp | 1a b,f | FR-2, FR-6, FR-7; toString() serves 1a i / FR-8 |
| src/MapGenerator.h | 1b 1,2 | FR-9 (automatic), FR-10 (random), FR-11 (structural path/kind guarantee, ADR 0003), FR-12 (seed); structurally satisfies 1a c,d (FR-3, FR-4) |
| src/MapGenerator.cpp | 1b 1,2 | FR-9, FR-10, FR-11, FR-12; structurally satisfies 1a c,d (FR-3, FR-4) |
| src/MapGeneratorConfig.h | 1a b + 1b 2 | FR-2 (size bounds, kMin* constants), FR-10 (tunables); helper-config exception to one-class-per-header (GR-2) |
| src/Demo.h | 1b 1 | FR-8 (console demo), FR-9 (automatic run); demo behaviour behind run_demo(std::ostream&) |
| src/Demo.cpp | 1b 1 | FR-8, FR-9 |
| src/main.cpp | 1b 1 | Thin wiring: run_demo -> std::cout (ADR 0004) |
| src/ItemBase.h | 1c 2a–e | FR-14..FR-18 (abstract base class of all items: name, description, weight > 0, value; ticket T2a, ADR 0006) |
| src/ItemBase.cpp | 1c 2a–e | FR-14..FR-18 (abstract base class of all items: name, description, weight > 0, value; ticket T2a, ADR 0006) |
| src/GoldCoin.h | 1c 2f | FR-19 (first concrete ItemBase child, the Aufgabe 1c gold coin; ticket T2a, ADR 0006) |
| src/GoldCoin.cpp | 1c 2f | FR-19 (first concrete ItemBase child, the Aufgabe 1c gold coin; ticket T2a, ADR 0006) |
| src/ItemFactory.h | 1c 3a,b | FR-19 (seeded deterministic catalog creating the 1c items; feeds FR-21/FR-43 in later tickets; ticket T2a, ADR 0006) |
| src/ItemFactory.cpp | 1c 3a,b | FR-19 (seeded deterministic catalog creating the 1c items; feeds FR-21/FR-43 in later tickets; ticket T2a, ADR 0006) |
| CMakeLists.txt | — | GR-5: out-of-source build entry (ADR 0001) |
| tests/ (tile_test.cpp, map_test.cpp, mapgenerator_test.cpp, map_rules_test.cpp, demo_test.cpp, item_test.cpp, docs_consistency.cmake) | — | GR-5: verification harness; shipped but the test suite, not graded program code |

## Tunables

Exactly as implemented in `src/MapGeneratorConfig.h`:

- `rows` / `cols` default 15x15; `kMinMapRows = kMinMapCols = 15`,
  enforced in `MapGenerator`'s config-taking constructor with a
  `std::invalid_argument` throw below the minimum (FR-2).
- `p_blocked = 0.3` (about 30% Blocked off the carved path, FR-10).
- `horizontal_step_cap = 3` (carve-path wiggle bound, FR-11).
- `min_blocked = 1` (all four kinds are always present, FR-1).

## Abgabe archive hygiene

The submitted archive (zip/rar/7z, uploaded via Moodle) contains NO
build/ directories: no build/, Debug/, or Release/ — builds are done
out-of-source (ADR 0001) and `build/` is gitignored. The archive ships
sources + CMake files + the written submission only. Clean the tree of
build artifacts (or build in a scratch directory) before zipping the
Abgabe submission.
