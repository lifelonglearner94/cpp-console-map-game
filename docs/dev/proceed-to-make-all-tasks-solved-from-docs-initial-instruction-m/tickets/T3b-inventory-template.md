### Ticket: T3b
id: T3b
title: Inventory class template (Aufgabe 2a)
description: Implement `src/Inventory.hpp` per ADR 0007: a header-only class template (templates ship in *.hpp, GR-1) whose template parameter is forwarded to a generic, vector-backed item container (FR-23, FR-24), with the slot count fixed at instantiation (FR-25), overflow refused with state unchanged (FR-26), index-based set/get/remove methods (FR-27), and three typed equipment members — slot 1 Ring, slot 2 Trousers, slot 3 Weapon, each accepting exactly its item class (FR-28).
acceptance criteria:
- src/Inventory.hpp compiles self-contained (included first in its test — GR-1)
- Template parameter forwarded to the container; wraps std::vector (FR-23, FR-24); type alias per ADR 0007
- Slot count fixed by constructor argument (FR-25); adding beyond capacity refused, state unchanged (FR-26)
- setSlot/getSlot/removeSlot with slot index (FR-27)
- Equipment slots accept exactly their item class; wrong type refused (FR-28)
- Tests instantiate the template with at least two pointer types (genericity, FR-23); overflow, bad-index, and wrong-type negative cases covered
- Full suite green
depends_on: T3a
