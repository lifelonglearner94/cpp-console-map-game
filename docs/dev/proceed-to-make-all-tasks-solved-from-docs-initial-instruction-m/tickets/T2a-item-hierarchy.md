### Ticket: T2a
id: T2a
title: Item hierarchy — ItemBase, GoldCoin, ItemFactory (Aufgabe 1c)
description: Implement the Aufgabe 1c item foundation per ADR 0006 and PLAN.md increment 2: abstract `ItemBase` (FR-14..FR-18: weight, name, description, value/price; weight must be > 0, constructor-validated, violations throw `std::invalid_argument`), the first concrete item class `GoldCoin` (FR-19), and a seeded, deterministic `ItemFactory` catalog (feeds FR-21 placement and FR-43 demo items). Header/definition separation and one class per header per the grading constraints; tests register beside the existing ones under the guarded-library pattern so the tree stays green.
acceptance criteria:
- src/ItemBase.h/.cpp: abstract base with weight/name/description/value accessors (FR-14..FR-18); weight <= 0 throws std::invalid_argument (FR-15)
- src/GoldCoin.h/.cpp: concrete ItemBase child (FR-19)
- src/ItemFactory.h/.cpp: seeded catalog, deterministic output for a given seed (ADR 0006)
- One class per header, English identifiers, no third-party libraries (GR-2..GR-4)
- Unit tests: weight validation (negative case), attribute accessors, describe(), factory determinism (same seed -> same item sequence)
- Test files registered via the guarded-library pattern; full suite green (step `test`: ctest --output-on-failure)
depends_on: (none)
