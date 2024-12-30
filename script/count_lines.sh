#!/bin/bash

lines="$(cloc --quiet --csv engine/src/ | tail -n1 | cut -d, -f5)"

LC_ALL="C.UTF-8" awk -v lines="$lines" 'BEGIN {printf "%6.1fk\n", lines/1000}' | sed 's/^ *//'
