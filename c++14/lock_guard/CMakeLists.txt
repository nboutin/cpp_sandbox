cmake_minimum_required(VERSION 2.6)

project(lock_guard)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

file(
  GLOB_RECURSE
  sources
  src/*
)

add_executable(
  lock_guard
  ${sources}
)

target_link_libraries(
  lock_guard
  pthread
)
