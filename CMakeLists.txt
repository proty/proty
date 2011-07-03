cmake_minimum_required(VERSION 2.8)

project(proty)
set(VERSION 0.2)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "--std=c99 -Wall")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-Wall")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")

# force out of source build
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
message(FATAL_ERROR "In-source builds are not allowed.
Please create a directory and run cmake from there, passing the path
to this source directory as the last argument.
This process created the file `CMakeCache.txt' and the directory `CMakeFiles'.
Please delete them.")
endif()

# default build type
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

# detect git version
exec_program(git ARGS describe OUTPUT_VARIABLE GIT_VERSION
                 RESULT_VARIABLE GIT_RETURN_VALUE)
if(NOT GIT_RETURN_VALUE EQUAL 0)
  set(GIT_VERSION "0")
endif(NOT GIT_RETURN_VALUE EQUAL 0)

add_subdirectory(compiler)
add_subdirectory(lib)

# doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)
    add_custom_target(doxygen ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile)
    add_custom_target(doc)
    add_dependencies(doc doxygen)
endif(DOXYGEN_FOUND)

# manpage
install(FILES ${CMAKE_SOURCE_DIR}/doc/man/proty.1 DESTINATION share/man/man1/)

# dist
set(ARCHIVE_NAME proty-${VERSION})
add_custom_target(dist
    COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
        | gzip > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.gz
        && gpg --armor --sign --detach-sign -o ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.gz.sig
           ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.gz
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
