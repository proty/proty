find_program(LLVM_CONFIG
  NAMES llvm-config
  PATHS
  /opt/local/bin
)

execute_process(COMMAND ${LLVM_CONFIG} --cxxflags
                        OUTPUT_VARIABLE LLVM_CXXFLAGS
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${LLVM_CONFIG} --ldflags
                        OUTPUT_VARIABLE LLVM_LDFLAGS
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${LLVM_CONFIG} --libs core jit interpreter linker native bitwriter
                        OUTPUT_VARIABLE LLVM_LIBS
                        OUTPUT_STRIP_TRAILING_WHITESPACE)

string(REGEX REPLACE "-O3" "" LLVM_CXXFLAGS ${LLVM_CXXFLAGS})

include_directories(include)

set(PREFIX ${CMAKE_INSTALL_PREFIX})
set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.hh.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/config.hh")

add_library(libproty SHARED
  lexer.cc
  parser.cc
  compiler.cc
  symtab.cc
  codegen.cc
  program.cc
)
target_link_libraries(libproty ${LLVM_LIBS})
set_target_properties(libproty PROPERTIES PREFIX ""
                               COMPILE_FLAGS ${LLVM_CXXFLAGS}
                               LINK_FLAGS "${LLVM_LDFLAGS}"
                               LINK_INTERFACE_LIBRARIES "")

add_executable(proty proty.cc)
target_link_libraries(proty libproty)

add_executable(iproty iproty.cc)
target_link_libraries(iproty libproty readline)

install(TARGETS proty RUNTIME DESTINATION bin)
install(TARGETS iproty RUNTIME DESTINATION bin)
install(TARGETS libproty LIBRARY DESTINATION lib)
