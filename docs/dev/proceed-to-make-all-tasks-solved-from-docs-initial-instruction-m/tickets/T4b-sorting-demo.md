### Ticket: T4b
id: T4b
title: SortingDemo executable (Aufgabe 3b)
description: Implement `src/SortingDemo.h/.cpp` with a `run_sorting_demo(std::ostream&)` seam plus a thin main adding the `sorting_demo` executable (same pattern as src/main.cpp): all six 3a sort functions demonstrated (FR-41) on inventories of 10-20 items (FR-42), chosen via seeded ItemFactory + fixed catalog so weights/names/values are pairwise distinct and correct sorting is recognizable (FR-43); each demonstration carries a header and shows the unsorted AND sorted inventory (FR-44).
acceptance criteria:
- sorting_demo executable builds and runs (registered beside the existing targets)
- All six combinations demonstrated (FR-41); each demonstrated inventory holds 10-20 items (FR-42)
- Items pairwise distinct in the sorted attribute per demonstration (FR-43)
- Output: header per demonstration + unsorted and sorted inventory (FR-44); deterministic given the seed
- Existing demos untouched; full suite green
depends_on: T4a
