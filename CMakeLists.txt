cmake_minimum_required(VERSION 3.4)

project(psutil_cpp)

set(BUILD_SHARED_LIBS OFF)

add_library(psutil_cpp 
    src/utils.cpp
    src/linux/psutil.cpp
)

target_include_directories(psutil_cpp
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
