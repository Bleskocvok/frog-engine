cmake_minimum_required(VERSION 3.18)

project(Frog)

message(STATUS "Build type:         '${CMAKE_BUILD_TYPE}'")

message(STATUS "Engine src folder:  ${CMAKE_CURRENT_SOURCE_DIR}")

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

message(STATUS "Generate documentation: ${FROG_GEN_DOCS}")



# ################################################
#
# --------------- Source files -------------------
#
# ################################################


file(GLOB
    SRC_FILES
    src/*.cpp
    src/*/*.cpp
    src/core/*.cpp
    src/geometry/*.cpp
    src/graphics/*.cpp
    src/os/*.cpp
    src/utils/*.cpp
)

# verbose info
print_list("Source files" "${SRC_FILES}")

file(GLOB
    SRC_TESTS
    tests/tests_main.cpp
    tests/tests_engine.cpp
    src/geometry/*.cpp
)

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


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# normal compiler options
if(NOT MSVC)
    add_compile_options(-Wall -Wextra -pedantic -O2 -Wold-style-cast)
else()
    # custom options for MSVC
    # because Microsoft is stupid
    add_compile_options(/W4 /Wall)
endif()

# debug compiler options
if(CMAKE_BUILD_TYPE MATCHES Debug)
    if(NOT MSVC)
        add_compile_options(-g -D DEBUG)
    else()
        # add_compile_options(/FI ${DEBUG_HEADER})
    endif()
endif()

# tests
set(TESTS tests)
add_executable(${TESTS} ${SRC_TESTS})

# window test
# add_executable(test_window WIN32 ${SRC_WINDOW_TEST})
# target_link_libraries(test_window PRIVATE ${LIST_OF_LIBRARIES})

# the resulting library
add_library(frog_lib ${SRC_FILES})
#target_link_libraries(frog_lib ${LIST_OF_LIBRARIES})
# alias, so that it games can link against Frog::Frog
add_library(Frog::Frog ALIAS frog_lib)



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


#
# Catch2
#
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY      https://github.com/catchorg/Catch2.git
        GIT_TAG             v2.13.6)
FetchContent_MakeAvailable(Catch2)
target_link_libraries(${TESTS} PRIVATE Catch2::Catch2)


#
# SDL2, SDL2_image, SDL2_mixer, SDL2_net
#

# tool for finding SDL2_*
FetchContent_Declare(
        sdl2-cmake-modules
        GIT_REPOSITORY      https://github.com/aminosbh/sdl2-cmake-modules.git
        GIT_TAG             ad006a3daae65a612ed87415037e32188b81071e)
FetchContent_MakeAvailable(sdl2-cmake-modules)

# add to modules
list(APPEND CMAKE_MODULE_PATH ${sdl2-cmake-modules_SOURCE_DIR})

# location of each SDL2 library
set(SDL2_PREFIX     "E:/development/SDL2/vs")
set(SDL2_PATH       "${SDL2_PREFIX}/SDL2-2.0.14"      CACHE BOOL "" FORCE)
set(SDL2_IMAGE_PATH "${SDL2_PREFIX}/SDL2_image-2.0.5" CACHE BOOL "" FORCE)
set(SDL2_MIXER_PATH "${SDL2_PREFIX}/SDL2_mixer-2.0.4" CACHE BOOL "" FORCE)
set(SDL2_NET_PATH   "${SDL2_PREFIX}/SDL2_net-2.0.1"   CACHE BOOL "" FORCE)

# find each needed lib
find_package(SDL2       REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_net   REQUIRED)

message(STATUS "Found SDL2:         ${SDL2_FOUND}")
message(STATUS "Found SDL2_MAIN:    ${SDL2MAIN_FOUND}")
message(STATUS "Found SDL2_Image:   ${SDL2_IMAGE_FOUND}")
message(STATUS "Found SDL2_Mixer:   ${SDL2_MIXER_FOUND}")
message(STATUS "Found SDL2_Net:     ${SDL2_NET_FOUND}")

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${SDL2_MIXER_INCLUDE_DIRS})
include_directories(${SDL2_NET_INCLUDE_DIRS})

# TODO

#target_link_libraries(${CLIENT_EXECUTABLE} SDL2::Main SDL2::Net SDL2::Image SDL2::Mixer)
#target_link_libraries(${SERVER_EXECUTABLE} SDL2::Main SDL2::Net)





target_link_libraries(frog_lib ${LIST_OF_LIBRARIES})
target_link_libraries(frog_lib SDL2::Main)




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

asset(assets/main.vert)
asset(assets/main.frag)
asset(assets/ui.vert)
asset(assets/ui.frag)
asset(assets/font.png)
asset(assets/empty.png)



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
    message(STATUS "Doxygen found:      ${DOXYGEN_VERSION}")

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
