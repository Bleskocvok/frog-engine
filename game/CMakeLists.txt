cmake_minimum_required(VERSION 3.18)

project(Game)

# use CMakeLists.txt from engine/
add_subdirectory(../engine ${CMAKE_CURRENT_BINARY_DIR}/engine)

set(SRC_FILES
    src/game.cpp
    src/powerup_script.cpp
    src/wall_script.cpp
 )

include_directories(${FROG_INCLUDE_DIRS})


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

if(NOT MSVC)
    # settings for the normal compilers
    add_compile_options(-Wall -Wextra -pedantic -O2)
else()
    # settings for MSVC
    add_compile_options(/W4 /Wall)
endif()

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Frog::Frog)


function(asset FILENAME)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
        ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
        COPYONLY
    )
endfunction()

asset(game_assets/ground.png)
asset(game_assets/heart.png)
