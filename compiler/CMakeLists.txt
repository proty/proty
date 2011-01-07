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
set(CMAKE_CXX_FLAGS_RELEASE "")

include_directories(include)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/config.hh.in" 
               "${CMAKE_CURRENT_SOURCE_DIR}/include/config.hh")

add_executable(proty
  src/lexer.cc
  src/parser.cc
  src/compiler.cc
  src/symtab.cc
  src/codegen.cc
  src/main.cc
)

target_link_libraries(proty ${LLVM_LIBS})
