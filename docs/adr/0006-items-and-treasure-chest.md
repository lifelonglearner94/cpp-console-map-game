# 0006 — Items: ItemBase hierarchy, treasure-chest tile, Map-owned item storage

## Status

Accepted (increment 2: Aufgabe 1c)

## Context

Aufgabe 1c extends 1b with:

1. a new tile type "treasure chest" (Konsole `t = treasure chest`);
2. a class `ItemBase` as the base class of all items, carrying
   *Gewicht* (weight, must be > 0), *Namen* (name), *Beschreibung*
   (description), *Wert/Preis* (value/price), plus at least one concrete
   derived item class;
3. items can lie on traversable tiles, every map carries at least 5
   items (they may be visible or hidden), and the map must still satisfy
   everything from 1a and 1b (all FR-1..FR-12 keep holding).

## Decision

- **`TileKind` gains `TreasureChest`, glyph `'t'`.** The kind is
  *traversable*: `Map::isTraversable` treats it like Traversable, so
  FR-3 paths (1a c) and later traversal are structurally unaffected.
  An emptied chest keeps the kind (an empty chest is still a chest).
- **`ItemBase` (`ItemBase.h/.cpp`) is an abstract base class**: name
  (`std::string`), description (`std::string`), weight (double, kg,
  constructor-validated > 0 — violations throw `std::invalid_argument`),
  value (double). A virtual `describe()` renders the item for console
  output. Concrete children:
  - `GoldCoin` (`GoldCoin.h/.cpp`) — 1c's required concrete class;
  - the 2a equipment classes `Ring`, `Trousers`, `Weapon`
    (see ADR 0007), each carrying a "+strength" attribute; `Weapon`
    additionally carries "+x% damage" (the assignment's documented
    optional bonus).
- **Item lifetime is owned by `Map`** (`std::vector<std::unique_ptr<ItemBase>>`);
  `Tile` stores a *non-owning* `const ItemBase*`. This keeps `Tile` a
  copyable value type (MapGenerator's `map.at(...) = Tile(...)`
  idiom survives) while `Map` answers `itemAt(row, col)` queries.
  Pick-up hands ownership from `Map` to the caller (unique_ptr release).
- **Placement is a new final `MapGenerator` step** (extending ADR 0003's
  pipeline without touching the carve logic): a seeded placement pass
  puts at least `item_count_min = 5` items on free traversable cells
  (a configurable share converted to `TreasureChest` tiles holding that
  item), then a *deterministic top-up* guarantees at least one item of
  each item class on every map — structural, no accept/reject loop.
- **`ItemFactory` (`ItemFactory.h/.cpp`)** owns the item catalog and
  creates seeded random items (`std::unique_ptr<ItemBase>
  create(std::mt19937&)`), so `MapGenerator` stays generation-logic
  only and item variety is configurable in one place.

## Rationale

- Non-owning tile pointer vs. item-in-Tile: a `std::unique_ptr` member
  would make `Tile` move-only and ripple through existing value
  semantics; the non-owning pointer adds one query without breaking any
  increment-1 API.
- Structural ≥5-item guarantee mirrors ADR 0003's philosophy (invariants
  by construction, testable over seed sweeps).
- A separate `ItemFactory` keeps one-class-per-header clean and gives
  3b/5a a single knob for generating item sets.

## Consequences / extension point

- New Map queries: `itemAt(row, col)` and an ownership-releasing pick-up
  hook used by 2b's `PlayerChar`; `Tile` gains `item()` / `setItem()`.
- The demo legend gains `t = treasure chest`.
- 2a's equipment classes slot straight into this hierarchy; no further
  Map/Generator redesign is needed for 2b/5a.
