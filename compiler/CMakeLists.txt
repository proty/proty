find_program(LLVM_CONFIG
  NAMES llvm-config
  PATHS
  /opt/local/bin
)

exec_program(${LLVM_CONFIG} ARGS --cxxflags OUTPUT_VARIABLE LLVM_CXXFLAGS)
exec_program(${LLVM_CONFIG} ARGS --ldflags OUTPUT_VARIABLE LLVM_LDFLAGS)
exec_program(${LLVM_CONFIG} ARGS --libs core jit interpreter linker native bitwriter OUTPUT_VARIABLE LLVM_LIBS)

string(REGEX REPLACE "-O3" "" LLVM_CXXFLAGS ${LLVM_CXXFLAGS})

set(CMAKE_EXE_LINKER_FLAGS ${LLVM_LDFLAGS})
set(CMAKE_CXX_FLAGS ${LLVM_CXXFLAGS} ${CMAKE_CXX_FLAGS})

include_directories(include)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.hh.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/config.hh")

add_executable(proty
  lexer.cc
  parser.cc
  compiler.cc
  symtab.cc
  codegen.cc
  main.cc
)

target_link_libraries(proty ${LLVM_LIBS})

install(TARGETS proty RUNTIME DESTINATION bin)
