### Ticket: T5e
id: T5e
title: Increment 5 seed sweep (FR-50) + docs guard + final green
description: Complete the increment-5 slice and the whole delivery: extend the seed sweep over GameConfig seeds to assert FR-50 on game-generated maps (>= 3 distinct items, >= 1 equipable — guaranteed by the per-class top-up) plus the underlying map invariants; extend `docs/source-task-map.md` + `tests/docs_consistency.cmake` shipped_files with rows per SPECS §7 for ConsoleRenderer, GameConfig, Game + its thin main, and the new game test files (FR-48..FR-62). Verify the full delivery is green and `game` runs interactively.
acceptance criteria:
- Seed sweep over GameConfig seeds asserts FR-50 (>= 3 distinct items, >= 1 equipable) and the map invariants on game maps
- Mapping rows added per SPECS §7 for every increment-5 file; existing rows untouched; shipped_files extended
- `game` runs interactively; the earlier demos still build and run
- Full suite green — the complete increments 2-5 delivery lands green on ctest (step `test`)
depends_on: T5a, T5b, T5c, T5d
