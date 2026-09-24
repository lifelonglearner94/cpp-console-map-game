# Smoke test: the demo executable must build, run, and exit 0.
add_test(NAME demo_runs COMMAND portfolio_game)

# Tile unit tests (ticket T2): hand-rolled assert-style, no external
# framework (GR-4). The tile sources (src/TileKind.h, src/Tile.h,
# src/Tile.cpp) are delivered by the implementation step; the guarded
# library link keeps `cmake` configurable until then, so the red state
# surfaces as the expected compile error (missing header) once the test
# target is registered.
add_executable(tile_test tile_test.cpp)
target_include_directories(tile_test PRIVATE "${CMAKE_SOURCE_DIR}/src")

if(EXISTS "${CMAKE_SOURCE_DIR}/src/Tile.cpp")
    add_library(tile STATIC "${CMAKE_SOURCE_DIR}/src/Tile.cpp")
    target_include_directories(tile PUBLIC "${CMAKE_SOURCE_DIR}/src")
    target_link_libraries(tile_test PRIVATE tile)
endif()

add_test(NAME tile_test COMMAND tile_test)

# Map unit tests (ticket T3): hand-rolled assert-style, no external
# framework (GR-4). The map sources (src/Map.h, src/Map.cpp) are
# delivered by the implementation step; the guarded library link keeps
# `cmake` configurable until then, so the red state surfaces as the
# expected compile error (missing header) once the test target is
# registered.
add_executable(map_test map_test.cpp)
target_include_directories(map_test PRIVATE "${CMAKE_SOURCE_DIR}/src")

if(EXISTS "${CMAKE_SOURCE_DIR}/src/Map.cpp")
    add_library(map STATIC "${CMAKE_SOURCE_DIR}/src/Map.cpp")
    target_include_directories(map PUBLIC "${CMAKE_SOURCE_DIR}/src")
    target_link_libraries(map_test PRIVATE map)
endif()

add_test(NAME map_test COMMAND map_test)

# MapGenerator unit tests (ticket T4): hand-rolled assert-style, no
# external framework (GR-4). The generator sources (src/MapGenerator.h,
# src/MapGenerator.cpp) are delivered by the implementation step; the
# guarded library link keeps `cmake` configurable until then, so the red
# state surfaces as the expected compile error (missing header) once the
# test target is registered. MapGenerator returns/uses p2game::Map, so
# the test links the map library too (Tile reaches it transitively via
# the root CMakeLists guarded map->tile link).
add_executable(mapgenerator_test mapgenerator_test.cpp)
target_include_directories(mapgenerator_test PRIVATE "${CMAKE_SOURCE_DIR}/src")

if(EXISTS "${CMAKE_SOURCE_DIR}/src/MapGenerator.cpp")
    add_library(mapgenerator STATIC "${CMAKE_SOURCE_DIR}/src/MapGenerator.cpp")
    target_include_directories(mapgenerator PUBLIC "${CMAKE_SOURCE_DIR}/src")
    target_link_libraries(mapgenerator PUBLIC map tile)
    target_link_libraries(mapgenerator_test PRIVATE mapgenerator)
endif()

add_test(NAME mapgenerator_test COMMAND mapgenerator_test)

# Map-rules sweep harness (ticket T5): hand-rolled assert-style, no
# external framework (GR-4). Sweeps ~100 seeds at 15x15 plus 20 seeds
# at 20x25 via MapGeneratorConfig and asserts the map rules on every
# generated map (plan step 5): min size, one Start in row 0 / one Exit
# in the last row, all four kinds with pairwise-distinct glyphs, and a
# start->exit path verified by a BFS that lives ONLY in this test file
# per the assignment's 4a separation rule (production stays
# pathfinding-free). All map/generator sources already exist (T1-T4),
# so this is registered unguarded; it links the mapgenerator library,
# which PUBLIC-links map and tile transitively.
add_executable(map_rules_test map_rules_test.cpp)
target_include_directories(map_rules_test PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(map_rules_test PRIVATE mapgenerator)

add_test(NAME map_rules_test COMMAND map_rules_test)

# Demo unit tests (ticket T6): hand-rolled assert-style main, no external
# framework (GR-4). The demo seam is int p2game::run_demo(std::ostream&)
# (src/Demo.h / src/Demo.cpp, delivered by the implementation step);
# run_demo renders Map glyphs, which need the map and tile libraries
# (Tile::glyph()), so the guarded demo library links both. The guard
# keeps `cmake` configurable while src/Demo.cpp is still missing, so the
# red state surfaces as the expected compile error on "Demo.h" in
# demo_test.cpp. The existing demo_runs smoke test stays untouched.
add_executable(demo_test demo_test.cpp)
target_include_directories(demo_test PRIVATE "${CMAKE_SOURCE_DIR}/src")

if(EXISTS "${CMAKE_SOURCE_DIR}/src/Demo.cpp")
    add_library(demo STATIC "${CMAKE_SOURCE_DIR}/src/Demo.cpp")
    target_include_directories(demo PUBLIC "${CMAKE_SOURCE_DIR}/src")
    target_link_libraries(demo PUBLIC map tile)
    target_link_libraries(demo_test PRIVATE demo)
endif()

add_test(NAME demo_test COMMAND demo_test)

# Documentation-consistency check (ticket T7): a pure script-mode CMake
# test (tests/docs_consistency.cmake) run via `cmake -P` — no compiler,
# no third-party deps (GR-3/GR-4) — so it is registered UNGUARDED like
# map_rules_test. It checks the documentation sweep against the code:
# docs/source-task-map.md exists and names every shipped file plus the
# Abgabe archive-hygiene rule, FR-9 is never conflated with min(imum)
# in the sources (FR-2 owns the minimum-size wording), and the dev
# SPECS/PLAN docs mention MapGeneratorConfig.h and Demo.cpp. It fails
# until the documentation step brings the docs in line.
add_test(NAME docs_consistency COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/tests/docs_consistency.cmake)
