SET(CMAKE_CXX_OUTPUT_EXTENSION ".bc")
SET(CMAKE_CXX_FLAGS "-emit-llvm")

add_library(vm.bc
  object.cc
  bool.cc
  integer.cc
  string.cc
  hash.cc
  list.cc
  class.cc
  function.cc
  type.cc
  instance.cc
  instructions.cc
)
