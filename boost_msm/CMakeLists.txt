cmake_minimum_required(VERSION 3.0.0)
project(boost_msm)

add_executable(${PROJECT_NAME}
	src/foo.cpp
	src/main.cpp)
	
target_include_directories(${PROJECT_NAME} PRIVATE src)

#target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
#target_link_libraries(${PROJECT_NAME} PRIVATE pthread rt)

find_package(Boost 1.58.0)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

message(STATUS ${Boost_INCLUDE_DIRS}) 