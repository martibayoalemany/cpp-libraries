# C++ libraries
[![StackShare](https://img.shields.io/badge/tech-stack-0690fa.svg?style=flat)](https://stackshare.io/graphai/graphai)

### Installation of libraries
* Add your libraries from conan.io/search in conanfile.txt
* Install a python3 environment with conan
* Compile with the following commands
```
source ~/.venv3/bin/activate
cd build
conan install .. -g cmake
cmake -C gcc_7_compiler.txt .
make
```

```
cd build
conan install .. -s compiler=clang -s compiler.version=4.0 -s compiler.libcxx=libstdc++ --build=missing
```
