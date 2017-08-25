#!/usr/bin/env bash

# COMPILER=${1:? Give the compile as a parameter i.e. gcc-7, gcc-6, clang}
if [ -n COMPILER ]; then
echo Compiler :  $COMPILER
else
export COMPILER=$1
fi


export CC=gcc-7
export CXX=g++

case $COMPILER in
gcc-4.9)
    export CC=gcc-4.9
    export CXX=g++-4.9
    ;;

gcc-5)
    export CC=gcc-5
    export CXX=g++-5
    ;;

gcc-6)
    export CC=gcc-6
    export CXX=g++-6
    ;;

clang-3.5)
    export CC=clang-3.5
    export CXX=clang++-3.5
    ;;

clang-3.6)
    export CC=clang-3.6
    export CXX=clang++-3.6
    ;;

clang-3.7)
    export CC=clang-3.7
    export CXX=clang++-3.7
    ;;

clang-3.8)
    export CC=clang-3.8
    export CXX=clang++-3.8
    ;;

clang-3.9)
    export CC=clang-3.9
    export CXX=clang++-3.9
    ;;

clang-4.0)
    export CC=clang-4.0
    export CXX=clang++-4.0
    ;;
clang)
    export CC=clang
    export CXX=clang++
    ;;
esac

echo $($CXX --version)

CMAKE_ARGS=(-DCMAKE_BUILD_TYPE=Debug  -DCMAKE_C_COMPILE=$CC -DCMAKE_CXX_COMPILER=$CXX -DCMAKE_CXX_FLAGS="-Werror -pedantic -O2")

echo cmake .. "${CMAKE_ARGS[@]}"