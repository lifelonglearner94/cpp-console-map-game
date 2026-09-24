### Ticket: T5
Title: Verification harness — map_rules_test sweep over sizes and seeds (plan step 5)
Description: Deliver the CTest-registered verification harness (tests/map_rules_test.cpp) that sweeps MapGenerator across ~100 seeds at 15×15 and larger sizes and asserts the plan's six verification conditions: (1) min size 15×15; (2) exactly one Start in row 0 and exactly one Exit in the last row; (3) the four kinds exist with distinct glyphs; (4) every kind present at least once; (5) a start→exit path exists, verified by a BFS kept ONLY in the test file; (6) seed A and seed B produce different maps. The BFS lives exclusively in the harness — Map and MapGenerator remain pathfinding-free per the 4a separation rule.
Acceptance criteria:
- tests/map_rules_test.cpp is registered with CTest and all tests pass green
- The sweep covers a representative seed range (≈100 seeds) at the 15×15 minimum and at least one larger size (FR-2)
- Every generated map has exactly one Start in row 0 and exactly one Exit in the last row (FR-4)
- Every generated map contains all four tile kinds with distinct glyphs (FR-1, FR-5)
- A traversable start→exit path is verified via BFS in the test harness only; BFS appears in no production file (FR-3, 4a separation)
- Generation output varies across seeds: two different seeds yield different maps (FR-10, FR-12)
- Harness stays English-identified, in the `p2game` namespace, and self-contained in tests/ (GR-3)
Depends on: T2, T3, T4
