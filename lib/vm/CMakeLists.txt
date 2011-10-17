add_library(protyvm SHARED
  block.c
)
target_link_libraries(protyvm protyrt)

install(TARGETS protyvm LIBRARY DESTINATION lib)
