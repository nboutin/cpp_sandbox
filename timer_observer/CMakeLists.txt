cmake_minimum_required(VERSION 3.0.0)
project(timer_observer)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE pthread rt)