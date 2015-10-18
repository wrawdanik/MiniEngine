#!/bin/bash

cd build

rm CMakeCache.txt

cmake -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_VERBOSE_MAKEFILE=true .. 

make clean
make