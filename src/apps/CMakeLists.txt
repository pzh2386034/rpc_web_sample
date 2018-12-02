###########################################
# CMake buil script for rpc sample apps
#
option(apps_build_tests "Build all of rpc api in rpc sample." ON)
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." ON)

file(GLOB sources
  "*.cpp"
  "*.c"
  )

link_directories("${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(rpcMain ${sources})
target_link_libraries(rpcMain com)
target_link_libraries(rpcMain pam)
