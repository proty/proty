include_directories(.)

add_subdirectory(runtime)
add_subdirectory(compiler)
add_subdirectory(vm)

add_library(io SHARED io/io.c io/file.c)
add_library(net SHARED net/net.c net/socket.c)
add_library(os SHARED os/os.c)
add_library(time SHARED time/time.c)

add_library(math SHARED math/math.c)
target_link_libraries(math m)

install(TARGETS
    io math# net os time
DESTINATION lib/proty)
