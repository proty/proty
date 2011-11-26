include_directories(.)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(BISON)
find_package(FLEX)

set(PREFIX ${CMAKE_INSTALL_PREFIX})
set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/config.h")

BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
FLEX_TARGET(Lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c
                  COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.h)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

add_library(protyc SHARED
  ${BISON_Parser_OUTPUTS}
  ${FLEX_Lexer_OUTPUTS}
  node.c
  compiler.c
  symtab.c
)
target_link_libraries(protyc protyvm)

install(TARGETS protyc LIBRARY DESTINATION lib)
