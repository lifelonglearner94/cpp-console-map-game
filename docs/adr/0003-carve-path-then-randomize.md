# 0003 — Map generation: carve a guaranteed path first, then randomize the rest

## Status

Accepted (increment: Aufgabe 1a/1b)

## Context

Aufgabe 1a c) requires a traversable path from start to exit on every
map; Aufgabe 1b requires the generation to be automatic and at least
partly random and/or procedural — while still satisfying every 1a
requirement. Naive random filling cannot guarantee 1a c); rejecting
bad maps in a loop works but is wasteful and gives poor-looking maps.

## Decision

`MapGenerator` uses a **carve-then-randomize** strategy:

1. **Carve a guaranteed path (the invariant core).**
   - Pick a random start column in row 0 and a random exit column in
     the last row.
   - Random walk from start with a *monotonically non-decreasing row*:
     at each step either step down (preferred, probability-weighted) or
     step left/right, with (a) no immediate backtracking, (b) a cap on
     consecutive horizontal steps (prevents infinite wandering in a
     row), and (c) never moving up. Monotone descent guarantees the
     walk reaches the last row in bounded steps, where it then moves
     horizontally to the exit column if needed.
   - Every visited cell is marked **Traversable**. This set of cells is
     a valid start→exit path by construction.

2. **Randomize the remaining cells (the variation).**
   - Every cell not on the carved path is filled randomly:
     Blocked with probability `p_blocked` (≈0.3), otherwise Traversable.
   - Because the fill only touches non-carved cells, the carved path
     survives *any* randomization — the 1a c) guarantee is structural,
     not probabilistic.

3. **Pin the required tile kinds.**
   - Carved path ⇒ Start, Exit and Traversable are guaranteed present.
   - Blocked is guaranteed by an explicit post-check: if the random fill
     produced zero (or too few) Blocked tiles, force a configurable
     minimum on off-path cells. Every generated map therefore contains
     all four required kinds (1a f).

4. **Seeding for testability.** The generator takes a seed (or a
   `std::mt19937`); the demo uses `std::random_device` by default and a
   fixed seed for reproducible output/tests.

## Rationale

- Guarantees 1a c) *by construction* instead of accept/reject loops.
- Randomized off-path cells plus a randomized path shape give maps that
  look genuinely random (1b) rather than like a thin corridor.
- Deterministic given a seed, so the verification harness can assert the
  invariants over many seeds.

## Consequences / extension point

- Aufgabe 4a pathfinding must stay a **separate module** (assignment:
  "Die Pathfinding Routinen müssen sauber von den map Routinen und
  Klassen getrennt werden"). `Map` only exposes traversal queries
  (`isTraversable(row, col)`, `rows()`, `cols()`); the generator's
  internal walk is not a pathfinding algorithm and lives only in
  `MapGenerator`.
- 1c can extend step 2 with a probability of placing items on
  traversable tiles without touching the carve logic.
- Tunables (size range, `p_blocked`, horizontal-step cap) become
  configuration, which is an assignment bonus area ("Mehr
  Konfigurationsmöglichkeiten/Mehr Attribute").
