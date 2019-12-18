cmake_minimum_required(VERSION 3.4)

project(psutil_cpp)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 17)

add_library(psutil_cpp 
    src/utils.cpp
    src/linux/psutil.cpp
)

target_include_directories(psutil_cpp
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

install(TARGETS psutil_cpp LIBRARY DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)