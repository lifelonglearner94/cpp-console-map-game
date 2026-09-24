### Ticket: T2
Title: TileKind enum + Tile value class — kinds and legend glyphs (FR-1, FR-5, GR-1/2/3)
Description: Deliver the smallest vertical slice of the domain: an enum-only TileKind.h declaring exactly Start, Exit, Blocked, Traversable (the assignment's permitted enum-only exception to one-class-per-header), and the Tile value class (Tile.h/Tile.cpp) carrying kind + display glyph with the legend glyphs s/e/b/x, Start and Exit distinct from every other tile. Wired into the build so the slice compiles and its behavior is verifiable on its own (e.g. a focused test or the placeholder main exercising Tile).
Acceptance criteria:
- TileKind is an enum-only header with exactly the four kinds Start, Exit, Blocked, Traversable, using the assignment's permitted enum-only exception (FR-1, GR-2)
- Tile is a value class with a kind and a display glyph; glyphs follow the legend s/e/b/x; Start and Exit glyphs are distinct from all other tiles' glyphs (FR-1, FR-5)
- Declarations live in Tile.h, definitions in Tile.cpp; include guards present; English identifiers; code sits in the `p2game` namespace (GR-1, GR-2, GR-3)
- The header includes only what it declares (no transitive leakage, per ADR 0005)
- Tile is shaped so Aufgabe 1c can later attach an item payload without changing the Map API shape (spec §8 extension point)
- The project still configures, builds, and runs green after the slice lands
Depends on: T1
