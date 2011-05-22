cmake_minimum_required(VERSION 2.6)

project(proty)
set(VERSION 0.1)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "--std=c99")
set(CMAKE_CXX_COMPILER "clang++")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib")

# default build type
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

# detect version
exec_program(git ARGS describe OUTPUT_VARIABLE GIT_VERSION)

add_subdirectory(compiler)
add_subdirectory(lib)

# install library files
# file(GLOB files "${CMAKE_SOURCE_DIR}/lib/*")
# install(FILES ${files} DESTINATION lib/proty)
