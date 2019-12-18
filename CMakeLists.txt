cmake_minimum_required(VERSION 3.5)

project(hello_library)

add_library(hello_library STATIC 
    src/utils.cpp
    src/linux/psutil.cpp
)

target_include_directories(hello_library
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
