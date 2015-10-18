#!/bin/bash

cd ../../build

rm CMakeCache.txt

cmake -DCMAKE_BUILD_TYPE=debug -DMINIENGINE_PLATFORM=osx .. 

make clean
make all 
make install
