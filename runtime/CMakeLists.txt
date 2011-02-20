set(CMAKE_C_LINK_EXECUTABLE "llvm-link <OBJECTS> -o <TARGET>")
set(CMAKE_C_FLAGS "-emit-llvm")

include_directories(include)

add_executable(lib/runtime.bc
  object.c
)

install(TARGETS lib/runtime.bc RUNTIME DESTINATION lib/proty)
