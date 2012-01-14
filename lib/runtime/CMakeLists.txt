add_library(protyrt SHARED
  object.c
  cfunction.c
  function.c
  hash.c
  list.c
  symbol.c
  integer.c
  float.c
  string.c
  nil.c
  bool.c
  exception.c
  runtime.c
)

install(TARGETS protyrt LIBRARY DESTINATION lib)

# install runtime headers
file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${files} DESTINATION include/proty/)
