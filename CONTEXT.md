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

## Glossary (increment 2+: items, inventory, characters, sorting,
pathfinding, game — ADRs 0006–0011)

- **Treasure chest** (`t`) — a fifth tile kind (Aufgabe 1c): traversable,
  holds an item; an emptied chest keeps its kind.
- **Item** — a game object lying on a traversable tile or carried in an
  inventory. Every item has name, description, weight (> 0, kg), and
  value/price (Aufgabe 1c).
- **ItemBase** — the abstract base class of all items (ADR 0006).
- **GoldCoin** — 1c's concrete item class.
- **Equipment items** — `Ring`, `Trousers`, `Weapon`, concrete ItemBase
  children, each carrying a "+strength" bonus; `Weapon` additionally
  "+x% damage" (Aufgabe 2a, ADR 0006/0007).
- **Hidden vs. visible item** — an item on a plain Traversable tile renders
  hidden (`x`); a chest-tile item is visible (`t`). The assignment permits
  both (1c: items "may be visible or hidden").
- **ItemFactory** — seeded catalog that creates random items (ADR 0006).
- **Inventory** — header-only class template over the item pointer type;
  fixed slot count, library-backed generic container, plus three concrete
  equipment slots (Aufgabe 2a, ADR 0007).
- **Item slot / equipment slot** — a numbered general-purpose slot vs. a
  slot that accepts exactly one equipment class (ring / trousers / weapon).
- **Character** — abstract base class for PC and NPC; defines the map
  traversal interface (Aufgabe 2b, ADR 0008).
- **PlayerChar** — the player character: moves on the map, owns a 10-slot
  Inventory + equipment slots, has a strength attribute (ADR 0008).
- **Strength / carry weight** — `maxWeight() = strength() × 2 kg` (10
  strength → 20 kg); equipping "+strength" items raises strength and thus
  the weight budget; while the carried weight is *exceeded*, the figure
  cannot move (Aufgabe 2b, ADR 0008).
- **Pick-up / drop / equip** — moving an item between a tile and an item
  slot, or into an equipment slot (Aufgabe 2b).
- **SortKey** — sorting dimension for inventories: weight, name, or value,
  each ascending or descending (Aufgabe 3a, ADR 0009).
- **Merge sort (ItemSorters)** — the hand-written stable sort used for all
  inventory sorting; `std::sort` stays out of the graded path (ADR 0009).
- **A\*** — the shortest-path algorithm from Start to Exit; lives in its
  own module and only *reads* the Map (Aufgabe 4a, ADR 0010).
- **Game** — the turn-based console game integrating Aufgaben 1–4:
  spawns a PlayerChar on Start, manual and automated traversal, goal =
  Exit (Aufgabe 5a, ADR 0011).
- **Manual traversal** — user-driven movement; item tiles offer pick-up /
  equip; inventory accessible any time.
- **Automated traversal** — the game walks the A\* path: auto-pick-up
  (free slot + strength budget permitting), auto-equip (better strength
  bonus per slot), path rendered as `*`, end-of-game inventory sort.
- **ConsoleRenderer** — renders the map plus overlays (`@` player, `*`
  path, `t` chest); owns all visualization so Map/Pathfinding stay pure
  (ADR 0011).

## Known grading constraints (from `docs/initial_instruction.md`)

- Mandatory code separation/structuring: declarations in `*.h`,
  definitions in `*.cpp`; templates in `*.h`/`*.hpp`.
- One class per header (helper classes / enum-only type declarations are
  the permitted exception); violations cost points.
- Identifiers may be in English or German; pick one and stay consistent.
- Third-party libraries are allowed where the task permits them — sources
  must always be referenced.

## Open questions (for `/domain-modeling` when it next runs)

- Both former open questions are now decided and recorded: rendering is
  console-first (ADR 0004, with `MapRenderer` as the future 2D swap point);
  `Map` is NOT templated (ADR 0002) — template competence is demonstrated
  by `Inventory` instead (ADR 0007).
- No new open questions from increment-2+ planning; ADRs 0006–0011 cover
  items, inventory, characters, sorting, pathfinding, and the game.
