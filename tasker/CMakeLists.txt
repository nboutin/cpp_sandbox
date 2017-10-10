cmake_minimum_required(VERSION 2.6)

project(tasker)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

file(
  GLOB_RECURSE
  sources
  src/*
)

add_executable(
  ${PROJECT_NAME}
  ${sources}
)

target_link_libraries(
  ${PROJECT_NAME}
  pthread
)
