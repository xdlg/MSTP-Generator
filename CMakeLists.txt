cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(MSTP-Generator)
add_compile_options(-Wall -Wextra -Ofast)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Image.cpp
    src/InputReader.cpp
    src/PatternGenerator.cpp
    src/VideoEncoder.cpp
)

target_link_libraries(${PROJECT_NAME}
    PkgConfig::LIBAV
)
