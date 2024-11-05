#!/bin/sh

cmake.exe -G "MinGW Makefiles" -S game -B bin-win
cmake.exe --build bin-win -j4
