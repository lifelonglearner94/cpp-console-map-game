# 0005 — Flat `src/` layout, English identifiers, one class per header

## Status

Accepted (increment: Aufgabe 1a/1b)

## Context

The assignment makes code separation and structuring mandatory and
grades it: declarations in `*.h`, definitions in `*.cpp` (templates:
`*.h`/`*.hpp`), and **one class per header** — the only permitted
exceptions are helper classes and enum-only type declarations.
Violations cost points. Identifiers may be English or German but must be
consistent.

## Decision

- **English identifiers** throughout (the seed glossary in `CONTEXT.md`
  is English and the domain terms map cleanly: Map, Tile, Start, Exit,
  Blocked, Traversable, Path, Generator).
- **Flat `src/` layout** with paired headers/sources, one class per
  header:

  ```
  CMakeLists.txt
  src/
    TileKind.h            // enum-only type declaration (permitted exception, no .cpp)
    Tile.h / Tile.cpp     // class Tile — one cell's kind (+ display glyph)
    Map.h / Map.cpp       // class Map — 2D grid of Tiles, size, traversal queries, toString()
    MapGenerator.h / MapGenerator.cpp // class MapGenerator — automatic, seeded generation (1b)
    main.cpp              // demo program
  tests/
    map_rules_test.cpp    // verification harness (CTest)
  ```

- No namespace proliferation: a single project namespace `p2game` wraps
  all project code.
- New classes in later increments each get their own `*.h`/`*.cpp` pair
  (or `*.hpp` when templated), never sharing a header.

## Rationale

- The layout makes the graded rules *structurally obvious*: any reviewer
  can check one-class-per-header at a glance.
- A flat layout is appropriate at this size (5 files); per-feature
  subdirectories can be introduced when the file count justifies it
  (e.g. `src/items/`, `src/pathfinding/` in later aufgaben).

## Consequences

- Aufgabe 4a's "pathfinding must be separated from map routines" will
  later be satisfied by a `src/pathfinding/` module (own headers), not
  by members of `Map`/`MapGenerator`.
- Header hygiene: headers include only what they declare; guards
  (`#ifndef`) on every header.
