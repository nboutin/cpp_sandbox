cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0009 NEW)

# Including custom cmake rules
include(cmake/clang-cxx-dev-tools.cmake)

project(cmake_helloworld)

set(EXECUTABLE_OUTPUT_PATH ../bin/${CMAKE_BUILD_TYPE})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(
  GLOB_RECURSE
  sources
  FOLLOW_SYMLINKS
  src/*
)

add_executable(
  ${PROJECT_NAME}
  ${sources}
)
