### Ticket: T7
Title: Documentation sweep — source→task mapping and increment-doc alignment (spec §7)
Description: Close increment 1 per the plan's step 7: update the written-submission source→task mapping (spec §7 Abgabe checklist) so each shipped file (TileKind.h, Tile.*, Map.*, MapGenerator.*, main.cpp) maps to Aufgabe 1a/1b, reconcile any drift between the spec/plan docs and the code as implemented (FR numbering, tunables, file list), and record the archive hygiene rule that no build/ directories ship in the submission.
Acceptance criteria:
- A source→task mapping exists covering every shipped file: TileKind.h, Tile.h/Tile.cpp, Map.h/Map.cpp, MapGenerator.h/MapGenerator.cpp, main.cpp (spec §7)
- The increment's spec and plan docs match the code as implemented (FR numbering, tunable values like p_blocked and size bounds, file list); any discrepancy found is fixed in the docs
- The archive hygiene rule (submission contains no build/ directories) is recorded in the mapping document (spec §7)
- The final build and full CTest run are green on the exact tree the documentation describes
Artifacts:
- docs/source-task-map.md (exists)
Depends on: T5, T6
