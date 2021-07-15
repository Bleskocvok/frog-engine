#!/bin/bash

bin/engine/tests

if [ $? ]; then

    bin/engine/test_window &
    pid=$!
    sleep 1 && kill "$pid"

    exit 0

fi

exit 1
