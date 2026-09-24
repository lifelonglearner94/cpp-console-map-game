# Domain Glossary — C++ Portfolio Game

Seeded during the per-repo skills setup (headless default). The glossary
is refined lazily by `/domain-modeling` as terms and decisions actually
get resolved — this file is a starting skeleton, not a finished spec.

## What this project is

A university "Programmieren 2" portfolio project (see
`docs/initial_instruction.md` for the assignment rules, which are the
source of truth for grading-relevant constraints). The basis is a
rudimentary tile-map game, delivered as portfolio tasks in Moodle.

## Glossary (initial terms from the assignment)

- **Map** — a 2D grid of tiles, minimum 15×15, implemented as a class
  (possibly templated, if that improves the implementation).
- **Tile** — one cell of the map; at least four tile kinds are required:
  - **Start** (`s`) — traversable entry tile; sits in the first row.
  - **Exit** (`e`) — traversable goal tile; sits in the last row.
  - **Blocked** (`b`) — impassable tile (blocks the path).
  - **Traversable** (`x`) — open path tile.
- **Path** — a traversable route from Start to Exit; every generated map
  must contain at least one.
- **Portfolio task** — one graded unit of the assignment (e.g. Aufgabe 1a).
- **Abgabe (submission)** — an archive (zip/rar/7z) uploaded to Moodle,
  containing the sources plus a written submission (pdf/word) that states
  which files implement which tasks. Build directories must be excluded.

## Known grading constraints (from `docs/initial_instruction.md`)

- Mandatory code separation/structuring: declarations in `*.h`,
  definitions in `*.cpp`; templates in `*.h`/`*.hpp`.
- One class per header (helper classes / enum-only type declarations are
  the permitted exception); violations cost points.
- Identifiers may be in English or German; pick one and stay consistent.
- Third-party libraries are allowed where the task permits them — sources
  must always be referenced.

## Open questions (for `/domain-modeling` when it next runs)

- Console-based rendering vs. 2D tile-asset rendering — which surface does
  the project target?
- Whether `Map` is templated, and over what (tile kind? dimensions?).
