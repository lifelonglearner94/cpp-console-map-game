# 0001 — CMake build system, C++17

## Status

Accepted (increment: Aufgabe 1a/1b)

## Context

The assignment permits either a Visual Studio Solution or a CMake project
for the portfolio solution ("Legen Sie eine Visual Studio Solution oder
ein CMake Projekt für die Portfolio Lösung an."). The development
environment is Linux/WSL2 with g++ 11.4.0, CMake 3.22.1, GNU Make 4.3 and
Ninja 1.10.1; Visual Studio (msbuild) is not available here. Later
aufgaben add tests and more executables (e.g. pathfinding demo in 4a),
so the build must scale to multiple targets cheaply.

## Decision

Use **CMake** (minimum 3.16) with **C++17** (`CMAKE_CXX_STANDARD 17`,
`CMAKE_CXX_STANDARD_REQUIRED ON`, no compiler extensions).

- One top-level `CMakeLists.txt` for increment 1; per-directory
  `CMakeLists.txt` files may be added as the project grows.
- Targets: the demo executable plus a test/verification executable
  registered with CTest.
- Out-of-source builds only (`build/` is git-ignored and never part of
  the Abgabe archive — assignment rule).

## Rationale

- C++17 is fully supported by g++ 11.4 and is sufficient for every
  planned feature (templates in Aufgabe 2a, `std::optional`/structured
  bindings if useful, `<random>`).
- CMake is explicitly permitted by the assignment, works with the
  available toolchain, and gives us CTest for the verification harness.

## Consequences

- The optional Windows binaries for the Abgabe can later be produced by
  building the same CMake project on Windows or cross-checked on a
  Windows machine — no Visual Studio solution has to be maintained.
- If the course's grader insists on opening a Visual Studio solution,
  a `.vcxproj`/solution can be generated from CMake later; no rework of
  sources is needed.
