#!/bin/bash

cd ../../build

rm CMakeCache.txt

cmake -DCMAKE_BUILD_TYPE=release -DMINIENGINE_PLATFORM=osx .. 


make
make install