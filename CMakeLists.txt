cmake_minimum_required(VERSION 2.8)

project(proty)
set(VERSION 0.1)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "--std=c99 -Wall")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-Wall")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")

# default build type
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

# detect version
exec_program(git ARGS describe OUTPUT_VARIABLE GIT_VERSION)

add_subdirectory(compiler)
add_subdirectory(lib)

# documentation
file(COPY ${CMAKE_SOURCE_DIR}/doc DESTINATION ${CMAKE_BINARY_DIR}/doc)

# doxygen
find_package(Doxygen)
configure_file(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)
add_custom_target(doxygen ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile)
add_custom_target(doc)
add_dependencies(doc doxygen)

# install library files
# file(GLOB files "${CMAKE_BINARY_DIR}/lib/*")
# install(FILES ${files} DESTINATION lib/proty)
