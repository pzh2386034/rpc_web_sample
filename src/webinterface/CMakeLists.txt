file(GLOB sources
  "*.c")

option(BUILD_SHARED_LIBS ON)
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
add_library(rpc_web SHARED ${sources})
