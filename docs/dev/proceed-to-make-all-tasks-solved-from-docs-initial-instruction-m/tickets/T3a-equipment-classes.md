### Ticket: T3a
id: T3a
title: Equipment item classes — Ring, Trousers, Weapon (Aufgabe 2a eq)
description: Extend the 1c item hierarchy per ADRs 0006/0007 with the three equipment classes that FR-28's dedicated slots accept: `Ring`, `Trousers`, `Weapon`, each a concrete ItemBase child carrying a `strengthBonus()` attribute (FR-28, FR-29); `Weapon` realizes the optional extra attribute as a "+x% damage" bonus (FR-29). Extend the `ItemFactory` catalog so the factory produces all three deterministically — this feeds the per-class top-up (FR-50) and the sorting demo (FR-43).
acceptance criteria:
- src/Ring.h/.cpp, src/Trousers.h/.cpp, src/Weapon.h/.cpp: concrete ItemBase children with strengthBonus() (FR-28, FR-29)
- Weapon carries a +damage% attribute (FR-29)
- One class per header; weight validation inherited from ItemBase (FR-15)
- ItemFactory catalog produces Ring/Trousers/Weapon, deterministic per seed
- Unit tests: strengthBonus accessors, Weapon damage bonus, factory determinism for the new classes
- Full suite green
depends_on: T2d
