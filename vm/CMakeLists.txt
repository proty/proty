set(CMAKE_CXX_LINK_EXECUTABLE "llvm-link <OBJECTS> -o <TARGET>")
set(CMAKE_CXX_FLAGS "-emit-llvm")

add_executable(vm.bc
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
