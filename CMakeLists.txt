cmake_minimum_required(VERSION 3.4)

project(psutil-cpp)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 17)

add_library(psutil-cpp 
    src/utils.cpp
    src/linux/psutil.cpp
)

target_include_directories(psutil-cpp
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

install(TARGETS psutil-cpp LIBRARY DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)