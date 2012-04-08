include_directories(../src/)

macro(proty_lib name)
  add_library(proty-${name} SHARED ${ARGN})
  set_target_properties(proty-${name} PROPERTIES
                                      RUNTIME_OUTPUT_DIRECTORY "proty"
                                      LIBRARY_OUTPUT_DIRECTORY "proty")
  target_link_libraries(proty-${name} libproty)
  install(TARGETS proty-${name} DESTINATION lib/proty)
endmacro(proty_lib)

proty_lib(io io/io.c io/file.c)
proty_lib(net net/net.c net/socket.c)
proty_lib(os os/os.c)
proty_lib(time time/time.c)
proty_lib(test test/test.c)

proty_lib(math math/math.c)
target_link_libraries(proty-math m)
