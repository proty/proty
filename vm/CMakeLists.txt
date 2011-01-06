set(CMAKE_CXX_LINK_EXECUTABLE "llvm-link <OBJECTS> -o <TARGET>")
set(CMAKE_CXX_FLAGS "-emit-llvm")

include_directories(include)

add_executable(vm.bc
  src/object.cc
  src/bool.cc
  src/integer.cc
  src/string.cc
  src/hash.cc
  src/list.cc
  src/class.cc
  src/function.cc
  src/type.cc
  src/instance.cc
  src/instructions.cc
)
