#!/usr/bin/env bash

#COMPILER=${1:? Give the compile as a parameter i.e. gcc-7, gcc-6, clang}
if [ -n COMPILER ]; then
echo Compiler :  $COMPILER
else
export COMPILER=$1
fi

set -e

docker rm -f cpp-libraries &>/dev/null || true
docker run -d -it --name cpp-libraries --privileged ubuntu:$UBUNTU
docker exec cpp-libraries mkdir cpp-libraries
docker exec cpp-libraries apt install python3 -y

docker cp . cpp-libraries:/cpp-libraries

docker exec cpp-libraries bash -c "
        python3 -m venv .venv3;
        source .venv3/bin/activate;
        pip install conan;
        cd cpp-libraries/build
        conan install .. -g cmake
"
docker exec cpp-libraries bash -c "
    cd cpp-libraries;
    scripts/cmake_helper.sh"
exit $?


