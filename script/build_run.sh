#!/bin/bash

clear;
time ./script/build.sh Debug && bin-lin/engine/tests && bin-lin/Game

