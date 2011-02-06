cmake_minimum_required(VERSION 2.6)

project(proty)
set(VERSION 0.1)

set(CMAKE_CXX_COMPILER "clang++")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib")

add_subdirectory(compiler)
add_subdirectory(vm)
add_subdirectory(lib)

# install library files
# file(GLOB files "${CMAKE_SOURCE_DIR}/lib/*")
# install(FILES ${files} DESTINATION lib/proty)
