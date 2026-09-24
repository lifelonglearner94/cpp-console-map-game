# Documentation-consistency check (ticket T7): a script-mode CTest run
# via `cmake -P tests/docs_consistency.cmake` — no compiler and no
# third-party deps (GR-3/GR-4). It guards the documentation sweep
# (increment 1 = Aufgabe 1a + 1b) against the code as shipped:
#   1. docs/source-task-map.md exists at the repo root.
#   2. The mapping file names every shipped source file.
#   3. The mapping file records the Abgabe archive-hygiene rule (no
#      build/ directories ship in the submission).
#   4. FR-numbering of the SOURCES: FR-9 (automatic generation,
#      Aufgabe 1b 1) is never conflated with min(imum) on one line —
#      FR-2 (Aufgabe 1a b) owns the 15x15 minimum-size references.
#      NOTE: FR-9 + 15x15 is legal (tests/demo_test.cpp pairs them: the
#      demo renders two 15x15 maps automatically), so the negative
#      pattern is FR-9 + min(imum) only, never FR-9 + 15x15.
#   5. Doc alignment: every SPECS.md / PLAN.md under docs/dev/*/ must
#      mention MapGeneratorConfig.h and Demo.cpp (both ship in src/).
# All checks run (failures are collected, never short-circuited): one
# [FAIL] line per failing check; the script exits non-zero iff at least
# one check failed, and prints a summary line when green.

cmake_minimum_required(VERSION 3.16)

set(repo_root "${CMAKE_CURRENT_LIST_DIR}/..")
set(failures "")

# ---- checks 1-3: docs/source-task-map.md ------------------------------
set(map_file "${repo_root}/docs/source-task-map.md")
set(shipped_files
    TileKind.h
    Tile.h
    Tile.cpp
    Map.h
    Map.cpp
    MapGenerator.h
    MapGenerator.cpp
    MapGeneratorConfig.h
    Demo.h
    Demo.cpp
    main.cpp
    src/ItemBase.h
    src/ItemBase.cpp
    src/GoldCoin.h
    src/GoldCoin.cpp
    src/ItemFactory.h
    src/ItemFactory.cpp
)

if(NOT EXISTS "${map_file}")
    list(APPEND failures
         "docs/source-task-map.md does not exist at the repo root")
else()
    file(READ "${map_file}" map_content)

    # (2) every shipped file must be named in the mapping
    foreach(name ${shipped_files})
        string(FIND "${map_content}" "${name}" idx)
        if(idx EQUAL -1)
            list(APPEND failures
                 "docs/source-task-map.md does not mention ${name}")
        endif()
    endforeach()

    # (3) Abgabe archive-hygiene rule: the literal build/ plus
    # abgabe or submission, matched case-insensitively
    string(FIND "${map_content}" "build/" idx_build)
    string(TOUPPER "${map_content}" map_upper)
    string(FIND "${map_upper}" "ABGABE" idx_abgabe)
    string(FIND "${map_upper}" "SUBMISSION" idx_submission)
    if(NOT (idx_build GREATER -1 AND (idx_abgabe GREATER -1 OR idx_submission GREATER -1)))
        list(APPEND failures
             "docs/source-task-map.md does not record the Abgabe archive-hygiene rule (needs a literal build/ plus abgabe or submission)")
    endif()
endif()

# ---- check 4: FR-9 must not be conflated with min(imum) ---------------
file(GLOB source_files RELATIVE "${repo_root}"
     "${repo_root}/src/*.h"
     "${repo_root}/src/*.cpp"
     "${repo_root}/tests/*.cpp")
foreach(src ${source_files})
    file(READ "${repo_root}/${src}" src_content)
    # Split the content into one list element per line. Real semicolons
    # are escaped first so string(REPLACE) splitting on newlines keeps
    # every original line intact as a single element.
    string(REPLACE ";" "\\;" lines_text "${src_content}")
    string(REPLACE "\n" ";" lines_text "${lines_text}")
    set(line_no 0)
    foreach(line IN LISTS lines_text)
        math(EXPR line_no "${line_no} + 1")
        string(REPLACE "\r" "" line "${line}")
        string(FIND "${line}" "FR-9" idx_fr9)
        if(idx_fr9 EQUAL -1)
            continue()
        endif()
        # min(imum)?\b case-insensitively: uppercase the line, then
        # require the MINIMUM match to end on a non-letter (CMake regex
        # has no \b, so [^A-Z] or end-of-line stands in for the boundary).
        string(TOUPPER "${line}" line_upper)
        string(REGEX MATCH "MIN(IMUM)?([^A-Z]|$)" min_match "${line_upper}")
        if(min_match)
            list(APPEND failures
                 "${src}:${line_no} conflates FR-9 (automatic generation) with min(imum) (FR-2 minimum size)")
        endif()
    endforeach()
endforeach()

# ---- check 5: SPECS.md / PLAN.md list the shipped files ---------------
file(GLOB dev_docs RELATIVE "${repo_root}"
     "${repo_root}/docs/dev/*/SPECS.md"
     "${repo_root}/docs/dev/*/PLAN.md")
foreach(doc ${dev_docs})
    file(READ "${repo_root}/${doc}" doc_content)
    foreach(name "MapGeneratorConfig.h" "Demo.cpp")
        string(FIND "${doc_content}" "${name}" idx)
        if(idx EQUAL -1)
            list(APPEND failures
                 "${doc} does not mention ${name}")
        endif()
    endforeach()
endforeach()

# ---- summary and exit status ------------------------------------------
list(LENGTH failures n_failures)
if(n_failures GREATER 0)
    foreach(reason IN LISTS failures)
        message("[FAIL] ${reason}")
    endforeach()
    message(FATAL_ERROR "docs_consistency: ${n_failures} check(s) FAILED")
endif()
message(STATUS "docs_consistency: all checks passed")
