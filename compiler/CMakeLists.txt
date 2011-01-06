find_program(LLVM_CONFIG
  NAMES llvm-config
  PATHS
  /opt/local/bin
)

exec_program(${LLVM_CONFIG} ARGS --cxxflags OUTPUT_VARIABLE LLVM_CXXFLAGS)
exec_program(${LLVM_CONFIG} ARGS --ldflags OUTPUT_VARIABLE LLVM_LDFLAGS)
exec_program(${LLVM_CONFIG} ARGS --libs core jit interpreter linker native bitwriter OUTPUT_VARIABLE LLVM_LIBS)


set(CMAKE_EXE_LINKER_FLAGS ${LLVM_LDFLAGS})
set(CMAKE_CXX_FLAGS ${LLVM_CXXFLAGS})

add_executable(proty
  lexer.cc
  parser.cc
  ast.cc
  compiler.cc
  main.cc
)

target_link_libraries(proty ${LLVM_LIBS})
