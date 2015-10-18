#!/bin/bash

cd build

rm CMakeCache.txt

cmake -DCMAKE_BUILD_TYPE=Release .. 

make clean
make