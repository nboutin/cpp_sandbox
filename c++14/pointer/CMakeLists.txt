cmake_minimum_required(VERSION 2.6)

project(pointer)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

file(
  GLOB_RECURSE
  sources
  src/*
)

add_executable(
  pointer
  ${sources}
)
