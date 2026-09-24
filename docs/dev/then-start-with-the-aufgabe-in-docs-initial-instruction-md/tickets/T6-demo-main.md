### Ticket: T6
Title: Demo main() — reproducible + random console map (FR-8, FR-9, FR-12)
Description: Replace T1's placeholder with the real Aufgabe 1b demo: main() generates one map with a fixed seed (reproducible assignment-style output) and one map with std::random_device-seeded randomness, prints the legend (s=Start, e=Exit, b=Blocked, x=Traversable) and the start/exit coordinates, then exits 0. Console ASCII rendering only — no raylib (ADR 0004, console-first).
Acceptance criteria:
- `./build/portfolio_game` renders a plausible 1b map matching the assignment's example format (spec §6 check 6) and exits 0
- One map is generated with a fixed seed and one with std::random_device-seeded randomness, so the demo shows both reproducibility and genuine randomness (FR-9, FR-12)
- The legend line explains s/e/b/x before any map is printed (FR-8, ADR 0004)
- Start and Exit coordinates are reported in the console output (FR-8)
- Console ASCII only; no raylib or windowing dependency appears in increment 1 (ADR 0004)
- The full CTest suite from T5 still passes green after the demo lands
Depends on: T5
