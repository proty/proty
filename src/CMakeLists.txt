add_executable(proty proty.c)
target_link_libraries(proty protyc)

add_executable(ipr ipr.c)
target_link_libraries(ipr protyc readline)

install(TARGETS proty RUNTIME DESTINATION bin)
install(TARGETS ipr RUNTIME DESTINATION bin)
