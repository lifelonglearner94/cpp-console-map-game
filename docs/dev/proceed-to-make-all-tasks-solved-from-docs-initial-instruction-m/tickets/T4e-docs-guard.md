### Ticket: T4e
id: T4e
title: Increment 4 docs guard + mapping
description: Complete the increment-4 slice: extend `docs/source-task-map.md` and `tests/docs_consistency.cmake`'s `shipped_files` with rows per SPECS §7 for SortKey.h, ItemSorters.hpp, SortingDemo + its thin main, Pathfinding.h/.cpp, PathfindingDemo + its thin main, and the new test files (3a/3b/4a rows).
acceptance criteria:
- Mapping rows added per SPECS §7 for every increment-4 file (FR-38..FR-47); existing rows untouched
- tests/docs_consistency.cmake shipped_files extended with the same files
- sorting_demo and pathfinding_demo still build and run; full suite green including the docs consistency test
depends_on: T4a, T4b, T4c, T4d
