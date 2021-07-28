#!/bin/sh

#files="src/ engine/src/ CMakeLists.txt engine/CMakeLists.txt engine/tests"

files="$(git ls-files | fgrep -v libraries)"

cloc $files

for i in $(seq 80); do printf '#'; done | 

scc $files
