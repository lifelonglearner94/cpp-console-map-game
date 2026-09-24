### Ticket: T5d
id: T5d
title: End-of-game inventory sort + display (Aufgabe 5a)
description: Complete the game's inventory features (FR-55, FR-62): at the end of the game the user can display the inventory and sort it in >= 2 ways — the six 3a combinations offered via the increment-4 sortInventory, rendered through the game's console output. No duplicate sort logic: the game calls ItemSorters.
acceptance criteria:
- End-of-game offers inventory display and sorting (FR-62); >= 2 sort ways selectable, all six 3a combinations offered (FR-55)
- Sorting delegates to sortInventory (ADR 0009/0011 — no duplicated sort logic)
- Display makes the sorted order recognizable (weights/names/values)
- Tests: a scripted session reaches the Exit, then sorts by two different keys and asserts the rendered order; works after both manual and automated traversal
- Full suite green
depends_on: T5b, T5c, T4a
