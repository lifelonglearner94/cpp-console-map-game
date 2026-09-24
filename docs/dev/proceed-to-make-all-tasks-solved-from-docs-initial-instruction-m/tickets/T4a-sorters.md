### Ticket: T4a
id: T4a
title: SortKey + ItemSorters — hand-written stable merge sort (Aufgabe 3a)
description: Implement `src/SortKey.h` (enum-only header, GR-2 exception) and `src/ItemSorters.hpp` per ADR 0009: a hand-written stable merge sort `mergeSort(It, It, Less)` plus `sortInventory(...)` offering all six combinations — weight/name/value x ascending/descending (FR-38..FR-40). No std::sort/std::stable_sort in the graded path; the library cross-check oracle lives in the test file only.
acceptance criteria:
- src/SortKey.h: enum-only sort-key declaration (GR-2 exception)
- src/ItemSorters.hpp compiles self-contained (included first in its test — GR-1)
- Stable merge sort hand-written; std::sort/std::stable_sort appear only in test files (FR-38..FR-40, ADR 0009)
- sortInventory offers all six combinations (FR-38, FR-39, FR-40)
- Tests cross-check against std::stable_sort as a TEST-ONLY oracle; empty, one-item, and full inventories covered; stability pinned (equal keys keep relative order)
- Full suite green
depends_on: T3d
