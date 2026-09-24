### Ticket: T1
Title: CMake scaffold — compilable, CTest-wired empty project (ADR 0001/0005)
Description: Lay the increment-1 foundation: a CMake ≥3.16 project pinned to C++17 with no compiler extensions, the flat src/ + tests/ layout of ADR 0005, a minimal compiling `portfolio_game` executable placeholder, CTest wired in, and a .gitignore for out-of-source builds. Every later ticket lands on a project that already configures and builds green.
Acceptance criteria:
- `cmake` configures out-of-source into a build/ directory and builds the `portfolio_game` target successfully (GR-5, ADR 0001)
- C++ standard is C++17 with compiler extensions off, per ADR 0001
- The demo executable runs and exits 0 (placeholder main), so the target is demoable on its own
- CTest is registered and `ctest` runs successfully with zero tests yet declared (wiring precedes tests)
- Directory layout matches ADR 0005: flat src/ for class files, tests/ for the harness; headers use include guards and the `p2game` namespace convention is established (GR-3)
- build/ output is ignored by version control via .gitignore
Artifacts:
- CMakeLists.txt (exists)
- .gitignore (exists)
- src/main.cpp (exists)
Verify expectation: runnable_zero_tests_ok
Depends on: none
