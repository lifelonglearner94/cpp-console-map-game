# 0011 — Game integration: turn-based console game with manual and automated traversal

## Status

Accepted (increment 5: Aufgabe 5a)

## Context

Aufgabe 5a verknüpft Aufgaben 1–4; alle dort definierten Beschränkungen
gelten weiter (traversierbare tiles, Gewichtsbeschränkung,
inventory/Itembeschränkungen). Required:

1. spawn a `PlayerChar` (Aufgabe 2) on the Start tile of a
   random/procedural map (requirements identical to 1b);
2. every map holds ≥ 3 distinct items, ≥ 1 of them equipable;
3. the program may be turn-based or real-time;
4. the user can traverse the map manually — with the figure visualized
   on the map, an offer to pick up and/or equip when entering an item
   tile, and inventory access at any time;
5. the user can traverse automatically via the pathfinding algorithm —
   with the taken path visualized, automatic pick-up on traversed item
   tiles (while free slots are available and the figure has enough
   strength), automatic equipping of items whose strength bonus is
   better than the currently equipped one for that slot, and an
   end-of-game inventory display + sort (≥ 2 ways);
6. the goal is reaching the Exit.

## Decision

- **Turn-based console game** (5a 3 allows either). Real-time would add
  a timer loop with no grading benefit; the console surface stays per
  ADR 0004. A turn = one user command.
- **New executable `game`** with a thin main wiring
  `int Game::run(std::istream&, std::ostream&)` — the same testable-seam
  pattern as `run_demo` / `run_sorting_demo` / `run_pathfinding_demo`.
  The per-Aufgabe demo executables (`portfolio_game`, `sorting_demo`,
  `pathfinding_demo`) stay untouched, so every Aufgabe remains
  individually demonstrable for the written Abgabe.
- **`Game` (`Game.h/.cpp`)** owns the session: a `Map` from
  `MapGenerator` (seeded), a `PlayerChar` spawned on the Start tile, the
  command loop, and the mode switches. `GameConfig.h` is a
  helper-config struct (seed, `MapGeneratorConfig`, base strength) —
  the same permitted exception as `MapGeneratorConfig.h` (GR-2).
- **`ConsoleRenderer` (`ConsoleRenderer.h/.cpp`)** renders a `const Map&`
  plus overlays: player glyph `'@'`, traversed path `'*'`, treasure
  chest `'t'` — precedence `@ > * > tile glyph`. Items lying on plain
  Traversable tiles render hidden (`x`) — permitted by 1c ("müssen aber
  nicht sichtbar sein"); the pick-up offer / auto-pick-up still fires on
  entry. Visualization lives in the renderer, not in `Map`,
  `Pathfinding`, or `Game` (extends ADR 0004; preserves ADR 0010's
  "the pathfinder draws nothing" end-to-end).
- **Manual mode**: compass-step movement validated by the `Character`
  interface (bounds + traversability + ADR 0008's weight rule);
  entering an item tile offers *pick up / equip / ignore*; `inventory`
  shows slots + equipment any time; a sort menu reuses the six 3a
  combinations (≥ 2 ways satisfied trivially); `auto` switches to
  automated traversal; `quit` ends the session. A manual pick-up that
  exceeds the weight budget is allowed (2b 7) — the game reports that
  the figure cannot move until something is dropped.
- **Automated mode**: `AStarPathfinder` computes the path from the
  *current* cell to the Exit (recomputed on entry, so a manually moved
  figure is handled); the game then steps cell by cell:
  - auto-pick-up only while a free item slot exists **and** the pick-up
    would not exceed `maxWeight()` (5a auto 2 a+b verbatim; over-weight
    pick-ups are skipped, never taken);
  - auto-equip when the traversed item's `strengthBonus()` beats the
    currently equipped item of its slot type; the displaced item
    returns to a free item slot per ADR 0008 — if no slot is free, the
    swap is skipped (documented policy);
  - every traversed cell is recorded and rendered as the `'*'` overlay
    (5a auto 1);
  - on reaching the Exit: summary + inventory display + the same sort
    menu (5a auto 4).
- **5a 2 (≥ 3 distinct items, ≥ 1 equipable) needs no new generator
  rule**: ADR 0006's deterministic top-up already guarantees ≥ 1 item
  of every class (GoldCoin, Ring, Trousers, Weapon) per map — 4
  distinct kinds, 3 of them equipable.

## Rationale

- Turn-based keeps every game session a deterministic function of
  (config, seed, input script) — testable through the `istream` seam
  with no timing flakiness.
- The auto-mode policies reuse ADR 0008's machinery unchanged; the only
  new rules are the two skip conditions, each tied to a literal
  assignment clause.
- One `Game` class plus one `ConsoleRenderer` keeps one-class-per-header
  clean and leaves NPCs (ADR 0008 extension) and a 2D renderer (ADR
  0004 extension) as pure additions.

## Consequences / extension point

- `game_test` drives scripted sessions through `run(std::istream&, ...)`:
  manual walk to exit, weight-blocked movement, auto pick-up/equip
  behaviour, end-of-game sort — plus a seed sweep for 5a 2.
- NPCs plug into `Character` without touching `Game`'s loop.
- The bonus 2D version swaps `ConsoleRenderer` behind the same call
  sites (ADR 0004's `MapRenderer` path).
