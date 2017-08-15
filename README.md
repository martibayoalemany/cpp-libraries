# C++ libraries
[![StackShare](https://img.shields.io/badge/tech-stack-0690fa.svg?style=flat)](https://stackshare.io/graphai/graphai)

### Installation of libraries
* Add your libraries in conanfile.txt
* Install conan and python3
* Compile with the command
```
source ~/.venv3/bin/activate
cd build
conan install .. -g cmake
cmake -C gcc_compiler.txt .
make
```
