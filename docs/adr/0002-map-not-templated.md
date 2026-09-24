# 0002 — Map is not templated in increment 1

## Status

Accepted (increment: Aufgabe 1a/1b)

## Context

Aufgabe 1a h) says: "Templetisieren Sie die map Klasse, falls Sie glauben,
dass dies die Implementierung verbessert" — templating the map class is
explicitly optional and conditioned on our own judgement. Templating
would force the whole Map implementation into a single `Map.hpp`
(assignment rule: templates are `*.h`/`*.hpp` only), removing the
classic `*.h`/`*.cpp` separation for the project's central class.

Later aufgaben make templating unavoidable elsewhere: Aufgabe 2a
requires a template-based `Inventory` with a generic container. So the
project demonstrates template competence regardless.

## Decision

**Do not template `Map` in this increment.** `Map` stores a fixed
element type `Tile` (a small value class holding a `TileKind`).

- Keep the element type named and centralized (`using tile_type = Tile;`
  inside `Map`) so a later `Map<T>` refactor stays cheap if it is ever
  wanted.
- The *seeded, deterministic generation and rendering* behaviour — the
  part that would be identical across element types — lives in
  `MapGenerator`, which is independent of the element type anyway.

## Rationale

- The requirement is optional ("falls Sie glauben"); a concrete map keeps
  the declaration/definition separation (`Map.h` + `Map.cpp`) clean,
  which is a *mandatory*, points-bearing rule.
- No concrete benefit is visible: every Aufgabe (1c treasure tiles,
  4a pathfinding, 5a traversal) operates on the same tile semantics.
- Template competence is demonstrated by `Inventory` (2a), which is
  mandatory there.

## Consequences / extension point

- If a future increment wants a templated `Map<T>`, the refactor is:
  rename `Map.h/Map.cpp` → `Map.hpp`, add the template parameter,
  move all definitions into the header. The public API
  (`at()`, `rows()`, `cols()`, `isTraversable()`, `toString()`) is
  designed so this is mechanical.
