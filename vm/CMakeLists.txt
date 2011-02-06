set(CMAKE_CXX_LINK_EXECUTABLE "llvm-link <OBJECTS> -o <TARGET>")
set(CMAKE_CXX_FLAGS "-emit-llvm")

include_directories(include)

add_executable(lib/vm.bc
  src/objects/object.cc
  src/objects/bool.cc
  src/objects/integer.cc
  src/objects/string.cc
  src/objects/hash.cc
  src/objects/list.cc
  src/objects/function.cc
  src/phash.cc
  src/instructions.cc
)

install(TARGETS lib/vm.bc RUNTIME DESTINATION lib/proty)
