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

message(STATUS "Generate documentation: ${FROG_GEN_DOCS}")



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
# Doctest
#
include_directories(libraries/doctest)



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
add_executable(tests ${SRC_TESTS})

# window test
# add_executable(test_window WIN32 ${SRC_WINDOW_TEST})
# target_link_libraries(test_window PRIVATE ${LIST_OF_LIBRARIES})

# the resulting library
add_library(frog_lib ${SRC_FILES})
target_link_libraries(frog_lib ${LIST_OF_LIBRARIES})
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
