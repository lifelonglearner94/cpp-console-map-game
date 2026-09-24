### Ticket: T3d
id: T3d
title: Increment 3 sweep extension + docs guard
description: Complete the increment-3 slice: extend the seed sweep so the per-class top-up is asserted across all four item classes (>= 1 GoldCoin/Ring/Trousers/Weapon per map — ADR 0006 top-up, which structurally guarantees FR-50's ">= 3 distinct items, >= 1 equipable"), adjusting the generator top-up additively if it does not yet cover the grown catalog; then extend `docs/source-task-map.md` + `tests/docs_consistency.cmake` shipped_files with the increment-3 rows per SPECS §7.
acceptance criteria:
- Seed sweep asserts >= 1 item of each of the four classes per generated map (=> FR-50 structural)
- All FR-1..FR-12 invariants and the increment-2 item invariants still hold across the sweep
- docs/source-task-map.md rows added per SPECS §7: Ring/Trousers/Weapon (FR-28/29), Inventory.hpp (FR-23..FR-29), Character (FR-30), PlayerChar (FR-31..FR-37), new test files; existing rows untouched
- tests/docs_consistency.cmake shipped_files extended with the same files
- Full suite green
depends_on: T2c, T3a, T3b, T3c
