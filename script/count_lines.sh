#!/bin/bash

set -eo pipefail


ARGS=". --exclude-dir=libraries,.github --vcs git"

1>&2 cloc $ARGS

lines="$(cloc --quiet --csv $ARGS | tail -n1 | cut -d, -f5)"

# LC_ALL="C.UTF-8" awk -v lines="$lines" 'BEGIN {printf "%6.1fk\n", lines/1000}' | sed 's/^ *//'
echo "$lines"
