### Ticket: T5a
id: T5a
title: ConsoleRenderer + GameConfig (Aufgabe 5a)
description: Implement `src/ConsoleRenderer.h/.cpp` per ADR 0011: renders a Map to an ostream with overlay precedence `@ > * > tile glyph` — the player figure `@` (FR-56), the automatically traversed path `*` (FR-59), and tile glyphs including the chest `t`; and `src/GameConfig.h` (helper struct: seed, MapGeneratorConfig, base strength) so every game session is a deterministic function of (GameConfig, seed, input script). The renderer reads Map via the public query surface and contains no game logic.
acceptance criteria:
- ConsoleRenderer renders any Map with `@` and `*` overlays; precedence `@ > * > tile glyph` pinned by tests
- Renderer is visualization-only: no game/traversal logic (ADR 0011 boundary)
- src/GameConfig.h carries seed, MapGeneratorConfig, base strength (helper-struct header, GR-2 exception)
- Unit tests: overlay precedence, `@`/`*` positions, plain map render unchanged from the increment-1 output
- Existing demos untouched; full suite green
depends_on: T4e
