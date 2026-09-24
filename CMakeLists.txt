cmake_minimum_required(VERSION 3.16)
project(p2game VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(portfolio_game src/main.cpp src/Demo.cpp src/Tile.cpp src/Map.cpp src/MapGenerator.cpp)

enable_testing()
add_subdirectory(tests)

# The map library is defined in tests/CMakeLists.txt, but Map.cpp stores
# Tile value cells and reads Tile::kind()/glyph(), whose definitions live
# in Tile.cpp (the tile library). Wire that link dependency here so
# map_test links; guarded so the red state (a missing source file) stays
# configurable.
if(TARGET map AND TARGET tile)
    cmake_policy(SET CMP0079 NEW)
    target_link_libraries(map PUBLIC tile)
endif()

# The demo library is defined in tests/CMakeLists.txt, but run_demo
# (src/Demo.cpp) builds its maps with MapGenerator::generate(), whose
# definition lives in MapGenerator.cpp (the mapgenerator library, which
# PUBLIC-links map and tile transitively). Wire that link dependency
# here so demo_test links; guarded so a missing demo/mapgenerator target
# keeps the tree configurable.
if(TARGET demo AND TARGET mapgenerator)
    cmake_policy(SET CMP0079 NEW)
    target_link_libraries(demo PUBLIC mapgenerator)
endif()
