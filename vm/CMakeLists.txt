set(CMAKE_CXX_LINK_EXECUTABLE "llvm-link <OBJECTS> -o <TARGET>")
set(CMAKE_CXX_FLAGS "-emit-llvm")

include_directories(include)

add_executable(lib/vm.bc
  src/object.cc
  src/bool.cc
  src/integer.cc
  src/string.cc
  src/hash.cc
  src/list.cc
  src/function.cc
  src/instructions.cc
)

install(TARGETS lib/vm.bc RUNTIME DESTINATION lib/proty)