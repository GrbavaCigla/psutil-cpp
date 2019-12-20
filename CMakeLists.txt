cmake_minimum_required(VERSION 3.4)

project(psutil-cpp DESCRIPTION "Psutil")

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 17)

string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_LOWER)


add_library(${PROJECT_NAME} 
    src/utils.cpp
    src/${SYSTEM_NAME_LOWER}/psutil.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib        
)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
