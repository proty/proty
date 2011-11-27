add_library(protyvm SHARED
  block.c
  eval.c
  module.c
  state.c
  opcodes.c
)
target_link_libraries(protyvm protyrt)

install(TARGETS protyvm LIBRARY DESTINATION lib)
