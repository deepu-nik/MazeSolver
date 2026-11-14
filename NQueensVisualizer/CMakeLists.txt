cmake_minimum_required(VERSION 3.16)

project(NQueensVisualizer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

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

add_executable(NQueensVisualizer
    src/main.c
    src/nqueens.c
)
target_include_directories(NQueensVisualizer PRIVATE include)
target_link_libraries(NQueensVisualizer PRIVATE raylib)

include(CTest)
if(BUILD_TESTING)
    add_executable(NQueensTests
        tests/test_nqueens.c
        src/nqueens.c
    )
    target_include_directories(NQueensTests PRIVATE include)
    add_test(NAME NQueensBacktracking COMMAND NQueensTests)
endif()
