### Ticket: T2d
id: T2d
title: Increment 2 docs guard + source-task-map rows
description: Complete the increment-2 slice: extend `docs/source-task-map.md` and `tests/docs_consistency.cmake`'s `shipped_files` with a row per file shipped in T2a-T2c, per the SPECS.md §7 table (1c rows). The Abgabe's written submission names files per task, so the mapping must cover every shipped file.
acceptance criteria:
- docs/source-task-map.md rows added per SPECS §7 for all increment-2 files (TileKind.h, Tile.h/.cpp, ItemBase, GoldCoin, ItemFactory, Map, MapGenerator + Config, Demo.cpp legend, new test files — FR-13..FR-22, FR-43); existing FR-1..FR-12 rows untouched (no renumbering)
- tests/docs_consistency.cmake shipped_files extended with the same files
- Every src/ and tests/ file shipped by T2a-T2c is named in the mapping
- Full suite green, including the docs consistency test
depends_on: T2a, T2b, T2c
