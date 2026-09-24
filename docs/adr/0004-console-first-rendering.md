# 0004 — Console rendering first; raylib deferred

## Status

Accepted (increment: Aufgabe 1a/1b)

## Context

The assignment offers a console-based game or a 2D-assets game, and
makes raylib an *optional* visualization for Aufgabe 1a i). The
console version is the baseline; a 2D version with own graphics is
listed as a bonus area. The dev environment has no raylib installed and
the first increment needs no graphics.

## Decision

Render increment 1 as **plain console output**: `Map::toString()`
produces the ASCII grid (`s`, `e`, `b`, `x` per the assignment legend);
`main()` prints the map plus the legend.

- No third-party dependency in increment 1 (also keeps the
  "Sourcen immer referenzieren" obligation trivially satisfied).
- Rendering is a *presentation concern*: it is one member function now,
  and extraction into a separate renderer class is pre-planned.

## Rationale

- Baseline requirement only; zero dependencies to reference or vendor.
- Keeps increment 1 small and verifiable.

## Consequences / extension point

- For raylib later: introduce a `MapRenderer` interface with a
  `ConsoleMapRenderer` (moved out of `Map`) and a `RaylibMapRenderer`
  implementation; `Map` stays presentation-free. `toString()` may remain
  as a convenience wrapper over the console renderer.
- One-class-per-header applies to the new renderer headers when they
  appear.
