# Session Context — Aufgabe 1a/1b first increment

## Operator's original request

> Then start with the aufgabe in docs/initial_instruction.md

## Orchestrator's scope decision for this session

Plan the **first delivery increment: Aufgabe 1a + 1b** (the assignment
explicitly allows them to be combined: "Aufgabe 1a und 1b dürfen zusammen
in einer Aufgabe bearbeitet werden!").

- Later aufgaben (1c, 2a, 2b, 3a, 3b, 4a, 5a) are **out of scope** for
  this increment, but the architecture must not foreclose them —
  extension points are called out in the spec.
- Assignment facts come from `docs/initial_instruction.md`, the source of
  truth. Read fully; relevant clauses summarized in SPECS.md.

## What this session must deliver

- `docs/dev/then-start-with-the-aufgabe-in-docs-initial-instruction-md/SPECS.md`
- `docs/dev/then-start-with-the-aufgabe-in-docs-initial-instruction-md/PLAN.md`
- ADR candidates in `docs/adr/`
- Decisions on: build system, C++ standard, directory layout
  (one-class-per-header), whether `Map` is templated, guaranteed
  start→exit path generation, demo `main()`, verification approach.

## Environment facts (verified this session)

- Linux (WSL2), x86_64, zsh. No Visual Studio, no clang, no raylib.
- g++ 11.4.0, CMake 3.22.1, GNU Make 4.3, Ninja 1.10.1 available.
- Greenfield repo: only `docs/initial_instruction.md`, `swe-agent.yaml`,
  `AGENTS.md`, `CONTEXT.md` (seed glossary), `docs/agents/`,
  `docs/adr/README.md`.
- Issues are tracked as local markdown under `.scratch/<feature-slug>/`
  (no remote/GitHub repo — see `docs/agents/issue-tracker.md`).

## Glossary vocabulary (from CONTEXT.md)

Map, Tile kinds Start/Exit/Blocked/Traversable, Path, Portfolio task,
Abgabe (submission).

## Grading hard constraints (from the assignment)

- Mandatory code separation/structuring: declarations in `*.h`,
  definitions in `*.cpp` for functions and classes; templates in
  `*.h`/`*.hpp`.
- One class per header; helper classes and enum-only type declarations
  are the permitted exception. Violations cost points.
- Identifiers English or German, but consistent.
- Third-party libraries only where the task permits; sources must be
  referenced.

## Submission-format constraints (operator's concern at Abgabe time — recorded, not built now)

- Moodle upload; archive must be zip/rar/7z; no build/Debug/Release
  directories in the archive; a written submission (pdf/word) listing
  which sources implement which task; optional Windows binaries in a
  separate folder.
