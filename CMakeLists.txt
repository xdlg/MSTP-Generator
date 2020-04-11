cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(MSTP-Generator)

include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(${PROJECT_NAME} src/main.cpp)
