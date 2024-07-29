cmake_minimum_required(VERSION 3.16)

project(MSTP-Generator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS Core Quick REQUIRED)
qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

add_subdirectory(src)
