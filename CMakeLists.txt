cmake_minimum_required(VERSION 3.16)

project(MazeSolver C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Fetch a fixed Raylib release so the project builds consistently across macOS and Windows.
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.2
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

add_executable(MazeSolver
    src/main.c
    src/maze.c
    src/bfs.c
)

target_include_directories(MazeSolver PRIVATE include)
target_link_libraries(MazeSolver PRIVATE raylib)
