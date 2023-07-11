#!/bin/bash

bin-lin/engine/tests

if [ $? ]; then

    bin-lin/engine/test_window &
    pid=$!
    sleep 1 && kill "$pid"

    exit 0

fi

exit 1
