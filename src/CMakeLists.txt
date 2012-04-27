find_package(BISON)
find_package(FLEX)

#
# libproty
#

add_library(libproty SHARED
  version.c
  vm/block.c
  vm/const.c
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
install(DIRECTORY vm DESTINATION include/proty FILES_MATCHING PATTERN "*.h")
install(DIRECTORY runtime DESTINATION include/proty FILES_MATCHING PATTERN "*.h")
install(DIRECTORY . DESTINATION include/proty FILES_MATCHING PATTERN "*.h")

#
# libprotyc
#

bison_target(Parser compiler/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
flex_target(Lexer compiler/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c
            COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.h)
add_flex_bison_dependency(Lexer Parser)

add_library(protyc SHARED
  ${FLEX_Lexer_OUTPUTS}
  ${BISON_Parser_OUTPUTS}
  compiler/node.c
  compiler/compiler.c
  compiler/symtab.c
)
target_link_libraries(protyc libproty)
install(TARGETS protyc LIBRARY DESTINATION lib)
install(DIRECTORY compiler DESTINATION include/proty FILES_MATCHING PATTERN "*.h")