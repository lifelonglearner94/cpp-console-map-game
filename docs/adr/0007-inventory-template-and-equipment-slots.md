# 0007 — Inventory: header-only template with concrete equipment slots

## Status

Accepted (increment 3: Aufgabe 2a; deliberately amends nothing in
ADR 0002 — `Map` stays non-templated)

## Context

Aufgabe 2a mandates an `inventory` class that

a) has a template-based class declaration whose template parameter is
   forwarded to the item container;
b) uses a generic (template-based) container for storing items — the
   container may itself wrap a library container;
c) fixes the number of item slots at instantiation;
d) enforces the maximum item count;
e) has methods to set and get items, at least one of which requires the
   slot index;

and then extends the same class with **3 dedicated equipment slots**,
each accepting exactly one specific item class (e.g. rings, trousers,
weapons) using three distinct item classes based on 1c's `ItemBase`,
each carrying a "+strength" attribute (more attributes optional,
e.g. "+x% damage").

## Decision

- **`src/Inventory.hpp`**: `template <typename ItemPtr> class Inventory`
  with `using container_type = std::vector<ItemPtr>;` — the template
  parameter lands directly in the library-backed container, satisfying
  2a a+b with one design move. Instantiated as
  `Inventory<std::shared_ptr<ItemBase>>` (shared ownership is needed
  because items are also referenced from map tiles); a raw-pointer
  instantiation stays possible for tests.
- **Slot semantics (2a c–e)**: the constructor takes the slot count
  (fixed at instantiation); `addItem` fills the first free slot,
  `setSlot(index, ...)` requires a free slot, `getSlot(index)` /
  `removeSlot(index)` work by index; capacity overflow is refused
  (returns false / leaves state unchanged); a bad index throws
  `std::out_of_range` (at-style). The maximum item count is therefore
  structurally enforced (2a d).
- **Equipment slots are concrete members of the same class template**
  (`ring_`, `trousers_`, `weapon_` of type `std::shared_ptr<Ring>`,
  `std::shared_ptr<Trousers>`, `std::shared_ptr<Weapon>`) with typed
  `equipX/unequipX` methods. Rationale: a "dedicated" equipment slot is
  by definition class-specific, so the assignment itself breaks
  genericity there; the *item container* (clause b) stays templated.
  Trade-off accepted and documented: an `Inventory` instantiated with
  any other element type still carries the (inert) equipment members.
- **Template form**: header-only `.hpp` (per the assignment's
  `*.h`/`*.hpp` template rule), include guard `P2GAME_INVENTORY_HPP`,
  no `.cpp`, fully self-contained (includes only ItemBase/Ring/
  Trousers/Weapon and std headers).
- **`Map` is untouched** — ADR 0002 stands; template competence is
  demonstrated by `Inventory` as increment-1's spec §8 anticipated.

## Rationale

- A template parameter of pointer type (not of item type) keeps the
  same header working for owning and non-owning instantiation and
  matches the item-lifetime design of ADR 0006.
- Typed equipment members are simpler and far more readable than a
  specialization or CRTP scheme, at zero grading risk.

## Consequences / extension point

- 2b's strength re-computation reads the three typed slots.
- 3a's sorters operate on the item container extracted from the
  Inventory; 5a reuses the same six sort combinations.
- A future "more item classes / more slots" bonus extends the catalog
  in `ItemFactory`, not the Inventory API.
