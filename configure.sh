#! /bin/sh

cmake -G "Unix Makefiles" -DASSIMP_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -S . -B build