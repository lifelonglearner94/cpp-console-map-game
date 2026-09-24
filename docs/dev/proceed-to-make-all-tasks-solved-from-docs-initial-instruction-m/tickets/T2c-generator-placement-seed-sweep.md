### Ticket: T2c
id: T2c
title: Generator item placement + extended seed sweep (Aufgabe 1c)
description: Wire items into map generation per ADR 0006: `MapGeneratorConfig.h` gains item tunables; `MapGenerator.cpp` gains a deterministic placement step that puts items on traversable tiles (FR-20) with a per-class top-up over the factory catalog, guaranteeing >= 5 items per map (FR-21) and >= 1 chest; `Demo.cpp`'s legend gains `t` (ADR 0006). Extend the seed-sweep harness (map_rules_test.cpp pattern, ~100 seeds x sizes) to assert every FR-1..FR-12 invariant on item maps (FR-22) plus the new item invariants.
acceptance criteria:
- MapGeneratorConfig.h: item tunables (per-class/total counts), deterministic given the seed
- MapGenerator.cpp: items placed only on traversable tiles (FR-20); every generated map carries >= 5 items (FR-21), >= 1 item per factory class, and >= 1 chest; chests traversable
- Demo.cpp legend shows `t` for TreasureChest
- Seed sweep asserts all FR-1..FR-12 invariants on item maps (FR-22) plus the item invariants above
- Placement/top-up is deterministic (same seed -> same map + items) and the generator stays a bounded random walk, never a search
- Full suite green
depends_on: T2a, T2b
