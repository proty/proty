set(CMAKE_C_LINK_EXECUTABLE "llvm-link <OBJECTS> -o <TARGET>")
set(CMAKE_C_FLAGS "-emit-llvm -fexceptions ${CMAKE_C_FLAGS}")

include_directories(.)

macro(proty_module TARGET SOURCE)
  add_custom_target(${TARGET} ALL
    COMMAND proty -o ${CMAKE_BINARY_DIR}/lib/${TARGET}
                  -m ${CMAKE_SOURCE_DIR}/lib/${SOURCE}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS proty
    COMMENT "Compiling proty module ${TARGET}"
    SOURCES ${SOURCE})
endmacro(proty_module)

add_executable(lib/runtime.bc
  runtime/object.c
  runtime/function.c
  runtime/hash.c
  runtime/list.c
  runtime/symbol.c
  runtime/integer.c
  runtime/float.c
  runtime/string.c
  runtime/nil.c
  runtime/bool.c
  runtime/exception.c
  runtime/runtime.c
)

add_executable(lib/io.bc
  io/io.c
  io/file.c
)

add_executable(lib/net.bc
  net/net.c
  net/socket.c
)

add_executable(lib/os.bc os/os.c)
add_executable(lib/math.bc math/math.c)
add_executable(lib/time.bc time/time.c)

proty_module(test.bc test.pr)

install(TARGETS
    lib/runtime.bc
    lib/io.bc
    lib/net.bc
    lib/os.bc
    lib/math.bc
DESTINATION lib/proty)

# install runtime headers
file(GLOB files "${CMAKE_SOURCE_DIR}/lib/runtime/*.h")
install(FILES ${files} DESTINATION include/proty/)
