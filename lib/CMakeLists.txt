set(CMAKE_C_LINK_EXECUTABLE "llvm-link <OBJECTS> -o <TARGET>")
set(CMAKE_C_FLAGS "-emit-llvm")
set(CMAKE_CXX_LINK_EXECUTABLE "llvm-link <OBJECTS> -o <TARGET>")
set(CMAKE_CXX_FLAGS "-emit-llvm")

include_directories(../vm/include)

add_executable(lib/runtime.bc
  runtime/object.c
  runtime/function.c
  runtime/symbol.c
  runtime/hash.c
  runtime/integer.c
  runtime/runtime.c
)

# add_executable(lib/io.bc io/io.cc)
