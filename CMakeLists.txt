cmake_minimum_required(VERSION 2.8)

project(proty C)
set(VERSION 0.4)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 4)
set(VERSION_PATCH 0)
set(COPYRIGHT "Copyright (C) 2010-2012 Thomas Gatzweiler")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -fPIC -Wall -Wno-unused-function")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_BUILD_RPATH "${LIBRARY_OUTPUT_PATH}/proty")
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/proty")

##### CPack
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})

set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_STRIP_FILES TRUE)
set(CPACK_GENERATOR "STGZ;TGZ;DEB;RPM")
set(CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Proty Maintainers")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libreadline6")

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
   set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
endif()

include(CPack)

###### FORCE OUT OF SOURCE BUILD
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
message(FATAL_ERROR "In-source builds are not allowed.
Please create a directory and run cmake from there, passing the path
to this source directory as the last argument.
This process created the file `CMakeCache.txt' and the directory `CMakeFiles'.
Please delete them.")
endif()

###### BUILD TYPE
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

###### GIT VERSION
execute_process(COMMAND git describe
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE GIT_VERSION
                RESULT_VARIABLE GIT_RETURN_VALUE
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET)

if(NOT GIT_RETURN_VALUE EQUAL 0)
  unset(GIT_VERSION)
endif(NOT GIT_RETURN_VALUE EQUAL 0)

###### CONFIG HEADER
set(PREFIX ${CMAKE_INSTALL_PREFIX})
set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(SHARED_LIB_PREFIX ${CMAKE_SHARED_LIBRARY_PREFIX})
set(SHARED_LIB_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
set(PLATFORM "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
configure_file("${CMAKE_SOURCE_DIR}/src/config.h.in"
               "${CMAKE_BINARY_DIR}/config.h")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${CMAKE_BINARY_DIR}/config.h")

###### SUBDIRECTORIES
add_subdirectory(src)
add_subdirectory(lib)

###### TESTS
add_custom_target(test
  COMMAND ${CMAKE_SOURCE_DIR}/test/test
          ${CMAKE_BINARY_DIR}/proty
          ${CMAKE_SOURCE_DIR}/test)

###### DOCUMENTATION
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc)
add_custom_target(doc)

###### DOXYGEN
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_SOURCE_DIR}/etc/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)
    add_custom_target(doxygen ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile)
endif(DOXYGEN_FOUND)

###### SPHINX
find_program(SPHINX_EXECUTABLE NAMES sphinx-build)

###### HTML DOCUMENTATION
add_custom_target(html-doc ${SPHINX_EXECUTABLE} -c ${CMAKE_SOURCE_DIR}/doc/
                           ${CMAKE_SOURCE_DIR}/doc/
                           ${CMAKE_BINARY_DIR}/doc/)
add_dependencies(doc html-doc)

###### PDF DOCUMENTATION
add_custom_target(latex-doc ${SPHINX_EXECUTABLE} -b latex
                            -c ${CMAKE_SOURCE_DIR}/doc/
                            ${CMAKE_SOURCE_DIR}/doc/
                            ${CMAKE_BINARY_DIR}/pdf-doc/)
add_custom_target(proty.pdf make -C ${CMAKE_BINARY_DIR}/pdf-doc/)
add_custom_target(pdf-doc ${CMAKE_COMMAND} -E copy
                          ${CMAKE_BINARY_DIR}/pdf-doc/proty.pdf
                          ${CMAKE_BINARY_DIR}/doc/proty.pdf)
add_dependencies(proty.pdf latex-doc)
add_dependencies(pdf-doc proty.pdf)
add_dependencies(doc pdf-doc)

###### MANPAGE
install(FILES ${CMAKE_SOURCE_DIR}/etc/man/proty.1 DESTINATION share/man/man1/)

###### SOURCE DIST
set(ARCHIVE_NAME proty-${VERSION})
add_custom_target(dist
    COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
        | gzip > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.gz
        && gpg --armor --sign --detach-sign -o ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.gz.sig
           ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.gz
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
