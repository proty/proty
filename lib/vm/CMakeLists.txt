add_library(protyvm SHARED
  block.c
  eval.c
  load.c
  module.c
  state.c
  opcodes.c
)
target_link_libraries(protyvm protyrt dl)

install(TARGETS protyvm LIBRARY DESTINATION lib)
