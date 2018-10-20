file(GLOB sources
  "*.c"
  "*.cpp")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
add_library(com ${sources})
