# 0008 — Character hierarchy: traversal interface and the weight rule

## Status

Accepted (increment 3: Aufgabe 2b)

## Context

Aufgabe 2b requires

1. a class `Character` as the base class for PC and NPC, defining an
   interface that enables traversing the map (see Aufgabe 1);
2. a class `PlayerChar` that can move on the map, owns an inventory
   (2a) with 10 item slots (+ equipment slots), and has a strength
   attribute that limits the maximum carryable weight (e.g. 10 str →
   max 20 kg);
3. once that carry weight is exceeded, the figure must not be movable
   on the map;
4. interfaces for picking up and dropping items and for equipping
   items (via the equipment slots);
5. when a "+strength" item is placed into an inventory slot, the
   `PlayerChar`'s strength attribute is adjusted accordingly.

## Decision

- **`Character` (`Character.h/.cpp`)** is an abstract base class holding
  the position (`row_`, `col_`) and defining the traversal interface on
  top of Map's existing query surface (spec §8 extension point):
  - `canEnter(const Map&, row, col)` — bounds check plus
    `Map::isTraversable(row, col)`;
  - `virtual bool moveTo(const Map&, row, col)` — validates, then
    commits the position; overridable by subclasses;
  - `virtual char glyph() const = 0` — for visualization (`'@'` for the
    player).
  NPCs derive later without touching `Map`.
- **`PlayerChar` (`PlayerChar.h/.cpp`)**: owns
  `Inventory<std::shared_ptr<ItemBase>> inventory_` constructed with
  **10 item slots** plus the three equipment slots; `strength()` is
  computed as `baseStrength_ + sum of equipped items' strengthBonus()`
  (default base strength 10), so equipping a "+strength" item adjusts
  the attribute live (2b 10) — and thereby the weight budget.
- **Weight rule**: `maxWeight() = strength() * kKgPerStrength`
  (`kKgPerStrength = 2.0`, named constant in the header → 10 str =
  20 kg, exactly the assignment's example); `carriedWeight()` sums the
  item weights held in *item slots* only (equipped items are worn, not
  carried). `moveTo` is overridden to refuse movement while
  `carriedWeight() > maxWeight()` (2b 7; at exactly the limit the
  character may still move — the clause says "wird ... überschritten").
- **Pick-up / drop (2b 8)**: `pickUpFrom(Map&, row, col)` takes a tile's
  item into the first free slot (even when this exceeds the budget —
  the character then simply cannot move, which is exactly what 2b 7
  prescribes); `dropTo(Map&, row, col)` releases a chosen slot's item
  onto a free tile. (5a's *automated* traversal additionally refuses
  auto-pick-up that would exceed the limit — "genug Stärke" there.)
- **Equip (2b 9)**: `equip(slot)` type-probes the item with
  `std::dynamic_pointer_cast` against Ring/Trousers/Weapon and calls
  the matching typed equipment slot; a swapped-out equipped item
  returns to a free item slot (equip fails when none is free).

## Rationale

- The traversal interface consumes only Map's read queries, keeping
  ADR 0002's separation (characters never modify Map).
- Computing strength from base + equipment (rather than mutating a
  stored strength on equip/unequip) makes the "+strength adjusts the
  attribute" clause provable and idempotent under repeated equip cycles.

## Consequences / extension point

- 5a's auto-equip compares a traversed tile's item `strengthBonus()`
  against the currently equipped item of the same slot — direct reuse.
- A future NPC class only implements `glyph()` (and behaviour); the
  movement/weight machinery stays in `PlayerChar`.
