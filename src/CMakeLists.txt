find_package(BISON)
find_package(FLEX)

include_directories(.)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/lib/)

#
# VM
#

add_library(libproty SHARED
  vm/block.c
  vm/eval.c
  vm/load.c
  vm/module.c
  vm/state.c
  vm/opcodes.c
  runtime/runtime.c
  runtime/object.c
  runtime/cfunction.c
  runtime/function.c
  runtime/hash.c
  runtime/list.c
  runtime/symbol.c
  runtime/integer.c
  runtime/float.c
  runtime/string.c
  runtime/nil.c
  runtime/bool.c
  runtime/exception.c
)
set_target_properties(libproty PROPERTIES OUTPUT_NAME "proty")
target_link_libraries(libproty dl)
install(TARGETS libproty LIBRARY DESTINATION lib)

#
# Compiler
#

BISON_TARGET(Parser compiler/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
FLEX_TARGET(Lexer compiler/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c
            COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.h)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

add_library(protyc SHARED
  ${FLEX_Lexer_OUTPUTS}
  ${BISON_Parser_OUTPUTS}
  compiler/node.c
  compiler/compiler.c
  compiler/symtab.c
)
target_link_libraries(protyc libproty)
install(TARGETS protyc LIBRARY DESTINATION lib)

#
# proty executable
#

add_executable(proty proty.c)
target_link_libraries(proty protyc)
set_target_properties(proty PROPERTIES LINK_FLAGS "-Wl,-rpath,${CMAKE_BUILD_RPATH}")
install(TARGETS proty RUNTIME DESTINATION bin)

#
# ipr executable
#

add_executable(ipr ipr.c)
target_link_libraries(ipr protyc readline proty-io)
set_target_properties(ipr PROPERTIES LINK_FLAGS "-Wl,-rpath,${CMAKE_BUILD_RPATH}")
install(TARGETS ipr RUNTIME DESTINATION bin)
