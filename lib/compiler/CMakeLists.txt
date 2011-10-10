find_package(BISON)
find_package(FLEX)

set(PREFIX ${CMAKE_INSTALL_PREFIX})
set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/config.h")

BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
FLEX_TARGET(Lexer lexer.l  ${CMAKE_CURRENT_BIANRY_DIR}/lexer.c)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

add_library(protyc SHARED
  lexer.c
  parser.c
  compiler.c
  symtab.c
  codegen.c
)
target_link_libraries(protyc protyrt)

install(TARGETS protyc LIBRARY DESTINATION lib)
