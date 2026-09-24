# Architecture Decision Records

Decisions about this codebase's architecture live here, one file per
decision, named `NNNN-short-title.md` (e.g. `0001-map-class-over-free-functions.md`).

ADRs are read by the engineering skills before working in an area (see
`docs/agents/domain.md`) and must be surfaced when a plan contradicts one.

## Index

- 0001 — CMake + C++17 build, out-of-source (`0001-cmake-build-cpp17.md`)
- 0002 — Map is a class, not templated (`0002-map-not-templated.md`)
- 0003 — MapGenerator: carve-path-then-randomize (`0003-carve-path-then-randomize.md`)
- 0004 — Console-first rendering, no windowing (`0004-console-first-rendering.md`)
- 0005 — Flat src/ layout, consistent English identifiers (`0005-layout-and-identifiers.md`)
- 0006 — Items: ItemBase hierarchy, treasure-chest tile, Map-owned item storage (`0006-items-and-treasure-chest.md`)
- 0007 — Inventory: header-only template with concrete equipment slots (`0007-inventory-template-and-equipment-slots.md`)
- 0008 — Character hierarchy: traversal interface and the weight rule (`0008-character-hierarchy-and-traversal.md`)
- 0009 — Inventory sorting: hand-written merge sort plus key/order comparators (`0009-inventory-sorting-module.md`)
- 0010 — Pathfinding: self-contained A*, query-only consumption of Map (`0010-pathfinding-astar.md`)
- 0011 — Game integration: turn-based console game, manual + automated traversal (`0011-game-integration-turn-based-console-game.md`)
