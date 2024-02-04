#!/bin/sh

# Run this script from the root folder of the project:
# (the folder where CMakeLists.txt is located)
#     $ script/build.sh build game2d Debug

set -e

[ "$#" -lt 2 ] && { echo "Usage: $0 [-o out_dir] [-b build_type] cmd dir"; exit 1; }

cmd="$1"
dir="$2"
build_type="$3"
out="bin-${dir}-lin"

build()
{
    cmake --build "$out" -j4
}

gen()
{
    cmake -D CMAKE_BUILD_TYPE="$build_type" -S "$dir" -B "$out"
}

case "$cmd" in
    build)
        build
        ;;
    gen)
        gen
        ;;
    gen-build)
        gen
        build
        ;;
    *)
        echo "Ivalid cmd: $cmd"
        exit 1
        ;;
esac
