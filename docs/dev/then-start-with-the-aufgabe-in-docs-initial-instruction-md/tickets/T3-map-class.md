### Ticket: T3
Title: Map class — 2D tile grid with traversal queries and toString() (FR-6, FR-7, FR-8 basis)
Description: Deliver the Map class (Map.h/Map.cpp): a 2D grid of Tile with rows()/cols() size accessors, at(row, col) cell access, isTraversable(row, col) — the traversal-query surface that Aufgabe 2b's Character interface and 4a's pathfinding will later consume — and toString() rendering the grid as console ASCII glyphs. Deliberately NOT templated (ADR 0002) and deliberately free of pathfinding, generation logic, and any I/O beyond toString(), per the spec's architecture section.
Acceptance criteria:
- Map is implemented as a class holding the tile grid, not bare arrays/functions (FR-6), and is not templated (FR-7, ADR 0002)
- Map exposes rows(), cols(), at(row, col), and isTraversable(row, col) as the stable query surface for later aufgaben 2b/4a (spec §8)
- toString() renders the map as console ASCII using the legend glyphs, the rendering basis for FR-8 (ADR 0004)
- Map contains no pathfinding and no generation logic; its only output is toString() (spec architecture boundary)
- Declarations in Map.h, definitions in Map.cpp; one class per header; include guards; `p2game` namespace (GR-1, GR-2, GR-3)
- The project still configures, builds, and runs green, with Map behavior verifiable on its own
Depends on: T2
