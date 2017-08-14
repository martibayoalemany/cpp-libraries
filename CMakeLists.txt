cmake_minimum_required(VERSION 3.2.2)

project(cpp-libraries)

#set(DEBUG true)

MESSAGE(CMAKE_BUILD_TYPE)

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
    set(GCC_COVERAGE_COMPILE_FLAGS " -g -O0 -Wall -lpthread -std=c++1z -ggdb -fconcept -v  -D_GLIBCXX_USE_CXX11_ABI=0 -fpermissive")
    MESSAGE("Not Using clang")
endif()


include(build/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB_RECURSE SOURCE_FILES src/*.cpp )
file(GLOB_RECURSE HEADER_FILES src/*.h )

list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/basics/FruitInst.cpp)
list(REMOVE_ITEM HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/basics/FruitInst.h)

foreach(var IN ITEMS ${SOURCE_FILES})
    MESSAGE("${var}")
endforeach()

foreach(var IN ITEMS ${CONAN_INCLUDE_DIRS})
    MESSAGE("${var}")
    include_directories(${var})
endforeach()

#include_directories(${HEADER_FILES})
add_executable(std_lib ${SOURCE_FILES} ${HEADER_FILES} src/utils/_DataFile.cpp src/utils/_DataFile.h src/utils/FileUtils.cpp src/utils/FileUtils.h)
	
#target_link_libraries(std_lib ${CONAN_LIBS})

