cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(EventBusTest)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
file(GLOB_RECURSE P_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h 
                           ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.hpp)

file(GLOB_RECURSE P_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

set(P_SAMPLE ${CMAKE_CURRENT_SOURCE_DIR}/sample/sample.cpp)

add_executable(${PROJECT_NAME} ${P_SAMPLE} ${P_SOURCE} ${P_HEADER})
