cmake_minimum_required(VERSION 3.18)

project(Frog)

message(STATUS "Build type: '${CMAKE_BUILD_TYPE}'")

message(STATUS "Engine src folder: \n        ${CMAKE_CURRENT_SOURCE_DIR}")

message(STATUS
"FROG
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⡆⠉⣔⡀⠀⡠⡀⢀⠄⠴⢶⢦⣴⣤⠄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⢀⠀⠃⠁⠀⠀⠐⢸⠸⠱⠈⣿⣿⣯⣉⣿⠃⠁⠂⡄⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠐⢤⣄⣀⣀⣀⡀⠈⠐⠒⠁⠈⢹⡟⠻⠿⢷⣒⠖⡅⡀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠙⠛⠛⢿⣴⡀⠀⠀⣠⡿⠃⠀⠀⠀⢻⡌⠚⠆⠚⠖⡀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⠙⠷⣿⣏⣀⡈⠐⠄⠀⡸⠧⠤⠤⠤⠀⠐⡀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢀⣾⡿⠁⠀⠀⠙⠿⣿⣷⠀⢸⣿⠄⣀⡀⠀⠀⠀⠀⡇⠀⠀
⠀⠀⠀⢀⣤⡴⢺⡿⡏⠀⠀⠀⠀⠀⣠⣾⡇⠀⡌⠁⠀⠈⠉⢱⣶⢀⠔⠁⠀⠀
⠀⠀⠀⠀⠉⠺⠿⠀⢸⡄⠀⠀⠀⠀⢘⡿⠁⠘⠀⠀⠀⠠⡺⡣⠂⠁⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢆⡠⠘⡳⠔⠾⠀⠀⠀⠀⠈⠈⠂⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠨⠤⠀⡄⠢⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
")


function(print_list LABEL LST)
    string(REPLACE
        ";"
        "\n        "
        str
        "\n        ${LST}"
    )
    string(REGEX REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/src/" "" str ${str})
    string(REGEX REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/"     "" str ${str})
    message(VERBOSE "${LABEL}:${str}")
endfunction()



# ################################################
#
# --------------- Options ------------------------
#
# ################################################

option(FROG_GEN_DOCS "Generate documentation" ON)
option(FROG_TEST     "Build engine tests"     ON)

option(FROG_BUILD_3D "Build engine for 3D game" ON)
option(FROG_BUILD_2D "Build engine for 2D game" ON)

message(STATUS "Generate documentation: ${FROG_GEN_DOCS}")
message(STATUS "Build 3D engine: ${FROG_BUILD_3D}")
message(STATUS "Build 2D engine: ${FROG_BUILD_2D}")



# ################################################
#
# --------------- Source files -------------------
#
# ################################################


set(SRC_FILES
    src/frog/core/engine2d.cpp
    src/frog/core/engine3d.cpp
    src/frog/core/object_builder.cpp
    src/frog/font/atlas.cpp
    src/frog/font/truetype.cpp
    src/frog/geometry/camera.cpp
    src/frog/geometry/general.cpp
    src/frog/geometry/objects.cpp
    src/frog/geometry/paddle.cpp
    src/frog/geometry/physics.cpp
    src/frog/geometry/polar.cpp
    src/frog/geometry/transform.cpp
    src/frog/graphics/renderer3d.cpp
    src/frog/gx3d/mesh.cpp
    src/frog/gx3d/model.cpp
    src/frog/gx3d/program.cpp
    src/frog/gx3d/shape.cpp
    src/frog/gx3d/texture.cpp
    src/frog/gx3d/vertex_objects.cpp
    src/frog/lib2d/error.cpp
    src/frog/lib2d/events.cpp
    src/frog/lib2d/font.cpp
    src/frog/lib2d/fs.cpp
    src/frog/lib2d/init.cpp
    src/frog/lib2d/structs.cpp
    src/frog/lib2d/texture.cpp
    src/frog/lib2d/time.cpp
    src/frog/lib2d/window.cpp
    src/frog/os/input.cpp
    src/frog/os/window.cpp
    src/frog/utils/exception.cpp
    src/frog/utils/fs.cpp
    src/frog/utils/ini.cpp
    src/frog/utils/string.cpp
)

# verbose info
print_list("Source files" "${SRC_FILES}")

set(SRC_TESTS
    tests/test_geometry.cpp
    src/frog/geometry/camera.cpp
    src/frog/geometry/general.cpp
    src/frog/geometry/objects.cpp
    src/frog/geometry/paddle.cpp
    src/frog/geometry/physics.cpp
    src/frog/geometry/polar.cpp
    src/frog/geometry/transform.cpp
)

set(SRC_STRING_TEST
    tests/test_string.cpp
    src/frog/utils/string.cpp
    src/frog/utils/ini.cpp
    src/frog/utils/exception.cpp
)

set(SRC_FIXED_TEST tests/fixed/test_fixed.cpp)
set(SRC_FIXED_BENCH tests/fixed/bench_fixed.cpp)

set(SRC_WINDOW_TEST
    tests/test_window.cpp
)

set(ENGINE_INCLUDE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    PARENT_SCOPE
)

set(DEBUG_HEADER
    "${CMAKE_CURRENT_SOURCE_DIR}/src/debug.hpp"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)



# ################################################
#
# ---------------- The executables ---------------
#
# ################################################


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# tests
if (FROG_TEST)
    add_executable(test_geometry ${SRC_TESTS})
    add_executable(test_fixed ${SRC_FIXED_TEST})
    add_executable(bench_fixed ${SRC_FIXED_BENCH})
    add_executable(test_string ${SRC_STRING_TEST})
endif(FROG_TEST)

# window test
# add_executable(test_window WIN32 ${SRC_WINDOW_TEST})
# target_link_libraries(test_window PRIVATE ${LIST_OF_LIBRARIES})

# the resulting library
add_library(frog_lib SHARED ${SRC_FILES})

# normal compiler options
if(NOT MSVC)
    target_compile_options(frog_lib PRIVATE -Wall -Wextra -pedantic -O2 -Wold-style-cast -fmax-errors=5)
else()
    # custom options for MSVC
    # because Microsoft is stupid
    target_compile_options(frog_lib PRIVATE /W4 /Wall)
endif()

# debug compiler options
if(CMAKE_BUILD_TYPE MATCHES Debug)
    if(NOT MSVC)
        target_compile_options(frog_lib PRIVATE -g -D DEBUG)
    else()
        # target_compile_options(frog_lib PRIVATE /FI ${DEBUG_HEADER})
    endif()
endif()



# ################################################
#
# --------------- Libraries setup ----------------
#
# ################################################

# needed for downloading the libraries
include(FetchContent)


set(LIST_OF_LIBRARIES "")
set(LIST_OF_INCLUDE_DIRS "")

macro(add_lib_to_list VALUE)
    list(APPEND LIST_OF_LIBRARIES "${VALUE}")
endmacro()

macro(add_dirs_to_includes VALUE)
    list(APPEND LIST_OF_INCLUDE_DIRS "${VALUE}")
endmacro()


if(FROG_BUILD_3D)
    #
    # OpenGL
    #
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIRS})
    add_dirs_to_includes("${OPENGL_INCLUDE_DIRS}")
    add_lib_to_list("${OPENGL_LIBRARIES}")


    #
    # Glad
    #
    add_library(glad libraries/glad/src/glad.c)
    set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)
    set(GLAD_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/libraries/glad/include")
    include_directories(${GLAD_INCLUDE_DIRS})
    add_dirs_to_includes("${GLAD_INCLUDE_DIRS}")
    add_lib_to_list(glad)


    #
    # GLFW
    #
    add_subdirectory(libraries/glfw-3.3.2
                    EXCLUDE_FROM_ALL)
    add_dirs_to_includes("${CMAKE_CURRENT_SOURCE_DIR}/libraries/glfw-3.3.2/include")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_lib_to_list(glfw)


    #
    # stb_image
    #
    include_directories(libraries/stb_image)
    add_dirs_to_includes("${CMAKE_CURRENT_SOURCE_DIR}/libraries/stb_image")
    add_library(stb_image libraries/stb_image/stb_image.c)
    add_lib_to_list(stb_image)
    set_target_properties(stb_image PROPERTIES POSITION_INDEPENDENT_CODE ON)


    #
    # tiny_obj_loader
    #
    FetchContent_Declare(
            tiny_obj_loader_git
            GIT_REPOSITORY      https://github.com/tinyobjloader/tinyobjloader.git
            GIT_TAG             v1.0.6)
    FetchContent_MakeAvailable(tiny_obj_loader_git)
    include_directories(${TINYOBJLOADER_INCLUDE_DIR})
    add_dirs_to_includes("${TINYOBJLOADER_INCLUDE_DIR}")
    add_lib_to_list(tinyobjloader)
else()
    target_compile_definitions(frog_lib PRIVATE -D NOT_FROG_BUILD_3D=1)
endif(FROG_BUILD_3D)


if(FROG_BUILD_2D)
    set(SDL_PREFIX     "${CMAKE_CURRENT_SOURCE_DIR}/libraries")

    set(SDL_PATH       "${SDL_PREFIX}/SDL2-2.28.5")
    set(SDL_NET_PATH   "${SDL_PREFIX}/SDL2_net-2.2.0")
    set(SDL_TTF_PATH   "${SDL_PREFIX}/SDL2_ttf-2.20.2")
    set(SDL_IMAGE_PATH "${SDL_PREFIX}/SDL2_image-2.8.1")
    set(SDL_MIXER_PATH "${SDL_PREFIX}/SDL2_mixer-2.6.3")

    # set(SDL2_PATH       "" CACHE BOOL "" FORCE)
    # set(SDL2_NET_PATH   "" CACHE BOOL "" FORCE)
    # set(SDL2_IMAGE_PATH "" CACHE BOOL "" FORCE)
    # set(SDL2_MIXER_PATH "" CACHE BOOL "" FORCE)

    message(STATUS "SDL_PATH:       ${SDL_PATH}")
    message(STATUS "SDL_NET_PATH:   ${SDL_NET_PATH}")
    message(STATUS "SDL_TTF_PATH:   ${SDL_TTF_PATH}")
    message(STATUS "SDL_IMAGE_PATH: ${SDL_IMAGE_PATH}")
    message(STATUS "SDL_MIXER_PATH: ${SDL_MIXER_PATH}")

    set(SDL2MIXER_INSTALL   OFF)
    set(SDL2MIXER_SAMPLES   OFF)
    set(SDL2MIXER_OPUS      OFF)
    set(SDL2MIXER_FLAC      OFF)
    set(SDL2MIXER_MIDI      OFF)
    set(SDL2MIXER_MOD       OFF)
    set(SDL2MIXER_MP3       OFF)

    set(SDL2TTF_INSTALL     OFF)

    set(SDL_TEST            OFF)
    set(SDL_STATIC          ON)
    set(SDL_SHARED          ON)

    add_subdirectory(${SDL_PATH}       sdl2-bin)
    add_subdirectory(${SDL_NET_PATH}   sdl2-net-bin)
    add_subdirectory(${SDL_TTF_PATH}   sdl2-ttf-bin)
    add_subdirectory(${SDL_IMAGE_PATH} sdl2-image-bin)
    add_subdirectory(${SDL_MIXER_PATH} sdl2-mixer-bin)

    find_library(SDL2       SDL2)
    find_library(SDL2_image SDL2_image)
    find_library(SDL2_mixer SDL2_mixer)
    find_library(SDL2_net   SDL2_net)
    find_library(SDL2_ttf   SDL2_ttf)

    # target_compile_definitions(frog_lib PRIVATE -D SDL_TTF_HEADER_FOLDER=SDL2)

    target_link_libraries(frog_lib PRIVATE SDL2::SDL2)
    # SDL2_image::SDL2_image doesn't work for some reason (same for mixer below)
    target_link_libraries(frog_lib PRIVATE SDL2_image)
    target_link_libraries(frog_lib PRIVATE SDL2_mixer)
    target_link_libraries(frog_lib PRIVATE SDL2_ttf)

    set(SDL2_INCLUDE_DIRS "${SDL_PATH}/include"
                          "${SDL_IMAGE_PATH}/include"
                          "${SDL_TTF_PATH}/include")

    include_directories(${SDL2_INCLUDE_DIRS})
    # TODO: remove this shit
    add_dirs_to_includes("${SDL2_INCLUDE_DIRS}")
else()
    target_compile_definitions(frog_lib PRIVATE -D NOT_FROG_BUILD_2D=1)
endif(FROG_BUILD_2D)


#
# Doctest
#
include_directories(libraries/doctest)



# ################################################
#
# ------------------- Linkage --------------------
#
# ################################################



target_link_libraries(frog_lib PRIVATE ${LIST_OF_LIBRARIES})
# alias, so that it games can link against Frog::Frog
add_library(Frog::Frog ALIAS frog_lib)



# ################################################
#
# ------------- Export for outside use -----------
#
# ################################################


set(FROG_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${LIST_OF_INCLUDE_DIRS}
    PARENT_SCOPE
)
set(FROG_LIBRARIES
    ${LIST_OF_LIBRARIES}
    frog_lib
    PARENT_SCOPE
)

# verbose info
print_list("Frog engine libraries" "${LIST_OF_LIBRARIES}")

# verbose info
print_list("Frog engine includes" "${LIST_OF_INCLUDE_DIRS}")



# ################################################
#
# ------------- Files configurations -------------
#
# ################################################


function(asset FILENAME)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
        ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
        COPYONLY
    )
endfunction()


#
# The file list
#

set(FROG_3D_ASSETS
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/main.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/main.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/ui.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/ui.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/font.png
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/empty.png
    PARENT_SCOPE
)

# asset(assets/main.vert)
# asset(assets/main.frag)
# asset(assets/ui.vert)
# asset(assets/ui.frag)
# asset(assets/font.png)
# asset(assets/empty.png)



# ################################################
#
# ------------- Documentation --------------------
#
# ################################################


if(FROG_GEN_DOCS)

find_package(Doxygen
             COMPONENTS dot)

if(DOXYGEN_FOUND)
    # print versions
    message(STATUS "Doxygen found: ${DOXYGEN_VERSION}")

    message(STATUS "DOT_PATH: ${DOT_PATH}")

    if(DOXYGEN_HAVE_DOT)
        message(STATUS " +  DOT found: ${DOXYGEN_DOT_VERSION}")
    else()
        message(STATUS " +  DOT not found")
    endif(DOXYGEN_HAVE_DOT)

    set(DOXYGEN_CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)

    add_custom_target(documentation ALL
                      COMMAND               Doxygen::doxygen ${DOXYGEN_CONFIG}
                      WORKING_DIRECTORY     ${CMAKE_CURRENT_SOURCE_DIR}/docs
                      COMMENT               "Target for documentation"
                      VERBATIM)

else()
    message(STATUS "Doxygen not found - ignoring documentation generation")
endif(DOXYGEN_FOUND)

endif(FROG_GEN_DOCS)
