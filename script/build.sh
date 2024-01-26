#!/bin/sh

# Run this script from the root folder of the project:
# (the folder where CMakeLists.txt is located)
#     $ script/build.sh

cmake -D CMAKE_BUILD_TYPE="$1"  \
      -S . -B bin-lin        \
    && cmake --build bin-lin -j4

