### Ticket: T3c
id: T3c
title: Character interface + PlayerChar (Aufgabe 2b)
description: Implement `src/Character.h/.cpp` — the abstract base for PC and NPC defining the map-traversal interface `canEnter`/`moveTo`/`glyph()` (FR-30, ADR 0008) — and `src/PlayerChar.h/.cpp` (FR-31..FR-37): validated movement via the interface (out-of-bounds/Blocked refused), an Inventory with 10 item slots plus the equipment slots (FR-32), strength limiting carry weight via `maxWeight() = strength() * kKgPerStrength` (2 kg per strength, FR-33), movement refused only while weight is *exceeded* (FR-34), pick/drop via `pickUpFrom`/`dropTo` (FR-35), `equip` with the displaced item returning to a free item slot (FR-36), and strength computed as base + equipped bonuses on every read (FR-37).
acceptance criteria:
- Character abstract with canEnter/moveTo/glyph(); PlayerChar moves only to traversable, in-bounds tiles (FR-30, FR-31)
- PlayerChar owns an Inventory with 10 item slots + 3 typed equipment slots (FR-32)
- maxWeight() = strength() * kKgPerStrength (FR-33); over-weight moveTo refuses, at-exactly-the-limit moveTo still moves (FR-34 boundary pinned by a test)
- pickUpFrom/dropTo transfer items between Map tiles and the inventory (FR-35)
- equip: displaced item returns to a free item slot; no free slot -> equip refused (FR-36)
- strength() always recomputes from equipped bonuses — never a stale copy (FR-37)
- Tests: traversal bounds/blocked/exit semantics, weight boundary, equip swap, strength recompute
- Full suite green
depends_on: T3b
