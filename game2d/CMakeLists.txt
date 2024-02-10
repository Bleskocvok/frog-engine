cmake_minimum_required(VERSION 3.22)

project(game_2d)

set(FROG_BUILD_3D OFF)
set(FROG_GEN_DOCS OFF)

add_subdirectory(../engine ${CMAKE_CURRENT_BINARY_DIR}/engine)

file(GLOB SRC_FILES
    src/*.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
include_directories(${FROG_INCLUDE_DIRS})

add_executable(game_2d ${SRC_FILES})
target_link_libraries(game_2d PRIVATE Frog::Frog)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# Normal compiler options
if(NOT MSVC)
    add_compile_options(-Wall -Wextra -pedantic -O2 -g)
else()
    # custom options for MSVC
    # because Microsoft is stupid
    add_compile_options(/W4 /Wall)
endif()


include(../engine/functions.cmake)

asset(assets/box.png)
asset(assets/circle.png)
