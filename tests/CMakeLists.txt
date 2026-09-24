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
