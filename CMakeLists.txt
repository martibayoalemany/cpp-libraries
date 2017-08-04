cmake_minimum_required(VERSION 3.2.2)

project(cpp14_basics)

if (DEBUG)
    MESSAGE("Cmake building in debug mode")
else()
    MESSAGE("Cmake building in release mode")
endif()

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)

message("COMPILER_ID : " ${CMAKE_CXX_COMPILER_ID})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")        
    set(CMAKE_CXX_FLAGS " -g -O0 -Wall -std=c++14")
    MESSAGE("Using clang")
else()
    set(GCC_COVERAGE_COMPILE_FLAGS " -g -O0 -Wall -lpthread -std=c++14 -ggdb")
    MESSAGE("Not Using clang")
endif()

include(build/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.h )
file(GLOB TEST_FILES src/test/*)

foreach(var IN ITEMS ${CONAN_INCLUDE_DIRS})
    MESSAGE("${var}")
    include_directories(${var})
endforeach()

include_directories(${HEADER_FILES})
add_executable(std_lib ${CONAN_INCLUDE_DIRS} src/stdLibInst.cpp src/main.cpp src/stdLibInst.cpp src/stdLibInst.h src/boostInst.cpp src/boostInst.h src/openCv.cpp src/openCv.h src/test/googleTestInst.cpp src/test/googleTestInst.h src/test/googleTestInst.cpp src/test/googleTestInst.h src/test/googleTestInst.cpp src/test/googleTestInst.h)
target_link_libraries(std_lib ${CONAN_LIBS})

