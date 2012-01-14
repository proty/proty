include_directories(${CMAKE_SOURCE_DIR}/lib/)

add_executable(proty proty.c)
target_link_libraries(proty protyc)
set_target_properties(proty PROPERTIES LINK_FLAGS "-Wl,-rpath,${CMAKE_BUILD_RPATH}")

add_executable(ipr ipr.c)
target_link_libraries(ipr protyc readline proty-io)
set_target_properties(ipr PROPERTIES LINK_FLAGS "-Wl,-rpath,${CMAKE_BUILD_RPATH}")

install(TARGETS proty RUNTIME DESTINATION bin)
install(TARGETS ipr RUNTIME DESTINATION bin)
