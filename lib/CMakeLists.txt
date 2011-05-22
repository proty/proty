set(CMAKE_C_LINK_EXECUTABLE "llvm-link <OBJECTS> -o <TARGET>")
set(CMAKE_C_FLAGS "-emit-llvm ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_LINK_EXECUTABLE "llvm-link <OBJECTS> -o <TARGET>")
set(CMAKE_CXX_FLAGS "-emit-llvm ${CMAKE_CXX_FLAGS}")

include_directories(.)

add_executable(lib/runtime.bc
  runtime/object.c
  runtime/function.c
  runtime/hash.c
  runtime/integer.c
  runtime/string.c
  runtime/runtime.c
)

add_executable(lib/io.bc io/io.c)
