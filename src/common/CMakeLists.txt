file(GLOB sources
  "*.c"
  "*.cpp")


option(BUILD_SHARED_LIBS ON)
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
add_library(com SHARED ${sources})
