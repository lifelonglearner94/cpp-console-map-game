cmake_minimum_required(VERSION 3.16)
project(p2game VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(portfolio_game src/main.cpp)

enable_testing()
add_subdirectory(tests)
