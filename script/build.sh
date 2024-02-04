#!/bin/sh

# Run this script from the root folder of the project:
# (the folder where CMakeLists.txt is located)
#     $ script/build.sh build game2d Debug

set -e

usage() { echo "Usage: $0 [-o out_dir] [-b build_type] cmd dir"; }

while getopts "hb:o:" op; do
    case $op in
        h) usage; exit 0;;
        b) build_type="$OPTARG";;
        o) out="$OPTARG";;
    esac
done
shift $(( $OPTIND - 1 ))

cmd="$1"
dir="$2"
[ -z "$out" ] && out="bin-${dir}-lin"
[ -z "$build_type" ] && build_type="Debug"

[ -z "$cmd" -o -z "$dir" ] && { usage; exit 1; }

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
