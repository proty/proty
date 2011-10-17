include_directories(.)

add_subdirectory(runtime)
add_subdirectory(compiler)
add_subdirectory(vm)

#add_executable(lib/io.bc io/io.c io/file.c)
#add_executable(lib/net.bc net/net.c net/socket.c)
#add_executable(lib/os.bc os/os.c)
#add_executable(lib/math.bc math/math.c)
#add_executable(lib/time.bc time/time.c)

#install(TARGETS
#    lib/runtime.bc
#    lib/io.bc
#    lib/net.bc
#    lib/os.bc
#    lib/math.bc
#    lib/time.bc
#DESTINATION lib/proty)
