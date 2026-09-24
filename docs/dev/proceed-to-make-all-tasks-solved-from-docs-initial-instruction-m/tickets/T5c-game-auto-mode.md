### Ticket: T5c
id: T5c
title: Automated traversal — pathfinding, auto-pick-up, auto-equip (Aufgabe 5a)
description: Extend Game with the automated mode (FR-53): traverse the map via AStarPathfinder, visualizing the path with the `*` overlay (FR-59); auto-pick-up items on traversed tiles only while (a) a free item slot exists and (b) the pick-up would not exceed maxWeight() — otherwise the item is skipped, never taken (FR-60); auto-equip an item whose strengthBonus() is better than the currently equipped item's for that slot type, skipping when the swap is impossible (no free item slot for the displaced item) (FR-61, ADR 0011 policy). The game integrates Pathfinding additively and never mutates Pathfinding/Map internals.
acceptance criteria:
- Auto command runs the A* path end-to-end; `*` overlay rendered (FR-53, FR-59)
- Auto-pick-up takes an item only when a slot is free AND weight stays within maxWeight(); both skip conditions pinned by tests (FR-60)
- Auto-equip swaps only on a strictly better strengthBonus() for that slot type; impossible swap skipped (FR-61)
- Game never mutates Pathfinding/Map internals (pinned by test)
- Tests: scripted auto-session covering path overlay, both FR-60 skips, better-bonus equip + impossible-swap skip
- Full suite green
depends_on: T5b, T4c
