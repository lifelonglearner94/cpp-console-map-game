# 0009 — Inventory sorting: hand-written merge sort plus key/order comparators

## Status

Accepted (increment 4: Aufgabe 3a + 3b)

## Context

Aufgabe 3a requires "Sortieralgorithmen" that sort inventory items by
weight, by name, and by value/price, each ascending and/or descending.
Aufgabe 3b requires a demonstration of all sorting functions on
artificially generated inventories of 10–20 items with
distinct-enough attribute values, with clear console output showing
the unsorted and the sorted inventory (stating what is demonstrated).
`std::sort` would satisfy the behaviour, but the clause says
*create* the algorithms.

## Decision

- **`src/ItemSorters.hpp`** — header-only free template functions:
  - a hand-written **stable merge sort** over iterators
    (`template <typename It, typename Less> void mergeSort(It, It, Less)`);
  - `template <typename ItemPtr> void sortInventory(Inventory<ItemPtr>&,
    SortKey, bool descending)` — builds the comparator from the
    `ItemBase` accessors (`weight()`, `name()`, `value()`) and applies
    the merge sort to a copy that is written back to the inventory.
- **`src/SortKey.h`** — `enum class SortKey { Weight, Name, Value }`
  (enum-only header, permitted exception like `TileKind.h`); the order
  is a `bool descending` argument, so each of the three keys is
  available in **ascending and descending** form — six combinations
  (a superset of the "aufsteigend und/oder abfallend" minimum).
- No `std::sort` in the graded path — the algorithm itself is shown and
  is trivially source-referencable ("Sourcen immer referenzieren").
- **`SortingDemo` (`SortingDemo.h/.cpp`)** behind
  `run_sorting_demo(std::ostream&)`: builds a seeded inventory of
  10–20 items from the `ItemFactory` catalog (weight/name/value drawn
  so values are pairwise distinct — name via a numbered suffix, the
  rest via disjoint value ranges), prints the unsorted inventory, then
  applies every key in both directions with a printed header stating
  what is being demonstrated, per 3b.

## Rationale

- A merge sort is short, provably O(n log n), stable, and easy to test
  exhaustively on small inventories.
- The sorters operate on the inventory domain only (spec §8: no Map
  impact) and are reusable by 5a's end-of-game sort.

## Consequences / extension point

- 5a's end-of-game inventory sorting reuses the same six combinations —
  "at least 2 ways" is trivially satisfied.
- Adding a sort key (e.g. description) means one more case in the
  comparator builder; the algorithm core is untouched.
