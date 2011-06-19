find_program(LLVM_CONFIG
  NAMES llvm-config
  PATHS
  /opt/local/bin
)

exec_program(${LLVM_CONFIG} ARGS --cxxflags OUTPUT_VARIABLE LLVM_CXXFLAGS)
exec_program(${LLVM_CONFIG} ARGS --ldflags OUTPUT_VARIABLE LLVM_LDFLAGS)
exec_program(${LLVM_CONFIG} ARGS --libs core jit interpreter linker native bitwriter OUTPUT_VARIABLE LLVM_LIBS)

string(REGEX REPLACE "-O3" "" LLVM_CXXFLAGS ${LLVM_CXXFLAGS})

include_directories(include)
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

add_executable(proty
  main.cc
)
target_link_libraries(proty libproty)

install(TARGETS proty RUNTIME DESTINATION bin)
