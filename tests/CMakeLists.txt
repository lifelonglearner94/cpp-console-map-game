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
