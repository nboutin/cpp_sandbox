cmake_minimum_required(VERSION 3.0.0)
project(args)

add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/arguments.cpp)
	
target_include_directories(${PROJECT_NAME} PRIVATE src)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

#target_link_libraries(${PROJECT_NAME} PRIVATE pthread rt)