### Ticket: T5b
id: T5b
title: Game core + manual traversal mode (Aufgabe 5a)
description: Implement `src/Game.h/.cpp` per ADR 0011 — `run(std::istream&, std::ostream&)` turn loop plus a thin main adding the `game` executable: spawn a PlayerChar on the Start tile of a generated map with 1b-identical requirements (FR-49), honoring all Aufgaben 1-4 constraints (FR-48); turn-based manual traversal (FR-51, FR-52) with the goal of reaching the Exit (FR-54); player visualized via the `@` overlay (FR-56); entering an item tile offers pick-up and/or equip (FR-57); the inventory is accessible at any time (FR-58). Weight/equipment rules come from PlayerChar — including that a manual over-weight pick-up is allowed and then blocks movement (FR-34).
acceptance criteria:
- `game` executable builds; Game::run(std::istream&, std::ostream&) drives a full session (testable via scripted streams)
- PlayerChar spawns on Start of a generated map (FR-49); turn-based manual moves; reaching Exit ends the game (FR-51, FR-52, FR-54)
- Manual map render shows `@` (FR-56); item-tile entry offers pick/equip/ignore (FR-57)
- Inventory viewable at any time (FR-58)
- Tests with scripted std::istream sessions: manual walk to exit; weight-blocked movement after an over-weight manual pick-up (FR-34); item-tile offer
- Full suite green
depends_on: T5a
