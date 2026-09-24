### Ticket: T4
Title: MapGenerator — carve-then-randomize, seeded generation (FR-2, FR-3, FR-4, FR-10, FR-11, FR-12)
Description: Deliver MapGenerator (MapGenerator.h/Map.cpp) implementing ADR 0003's carve-then-randomize: pick a random start column in row 0 and a random exit column in the last row (FR-4), carve a monotone-descent random walk marking Traversable so a start→exit path exists by construction (FR-3, FR-11), then randomize every off-path cell Blocked with p_blocked (≈0.3) else Traversable (FR-10), pinning all four kinds via a forced minimum on off-path cells if the random fill produced too few (FR-1). Seeded std::mt19937 makes it reproducible; configurable size (min 15×15, larger allowed) and p_blocked are generator configuration (FR-2, FR-12, spec §9 tunables).
Acceptance criteria:
- A plain generator run produces a complete, ready-to-use map without any input (FR-9)
- Generation is automatic and genuinely procedural/random: map shape varies between runs with different seeds (FR-10), yet every run satisfies all 1a constraints structurally, not probabilistically (FR-11, ADR 0003)
- Start tile is placed in row 0, Exit in the last row, at randomized columns (FR-4)
- The carved path guarantees a traversable start→exit path survives any randomization (FR-3, FR-11)
- Minimum map size is 15×15 with larger sizes supported and configurable; p_blocked and the horizontal-step cap are configuration, not constants scattered in code (FR-2, spec §9)
- A seed (or std::mt19937) parameter makes generation reproducible; all four tile kinds are guaranteed present (FR-1, FR-12)
- No pathfinding logic in the generator; its walk is not a pathfinding algorithm and stays confined to MapGenerator (assignment 4a separation rule)
- Declarations in MapGenerator.h, definitions in MapGenerator.cpp; one class per header; `p2game` namespace (GR-1, GR-2, GR-3)
- The project still configures, builds, and runs green, with deterministic-seed generation verifiable on its own
Depends on: T3
