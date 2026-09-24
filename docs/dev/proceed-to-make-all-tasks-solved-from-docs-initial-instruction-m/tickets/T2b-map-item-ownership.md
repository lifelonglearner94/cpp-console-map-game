### Ticket: T2b
id: T2b
title: TreasureChest tile kind, Tile item view, Map item ownership (Aufgabe 1c)
description: Extend the map layer for items per ADR 0006: `TileKind` gains `TreasureChest` — console glyph `t`, traversable like Traversable so the FR-3/FR-11 path guarantees stay structural (FR-13); `Tile` gains a non-owning item view (`item()`/`setItem()`); `Map` becomes the item owner (`std::vector<std::unique_ptr<ItemBase>>` storage) with an `itemAt()` query (FR-20). The existing public query surface (`rows()`, `cols()`, `at()`, `isTraversable()`, `toString()`) stays unchanged so increment-1 tests pass unmodified.
acceptance criteria:
- src/TileKind.h: TreasureChest kind with glyph `t`, traversable (FR-13)
- src/Tile.h/.cpp: non-owning item view accessors (item()/setItem())
- src/Map.h/.cpp: item ownership via unique_ptr storage + itemAt() query (FR-20); existing query surface unchanged
- Chest tiles are traversable; a hand-built map's path invariants are unaffected
- Unit tests: chest kind traversability, item set/get round-trip on a hand-built Map, itemAt() on an empty tile
- Full suite green; increment-1 map tests pass unmodified
depends_on: T2a
