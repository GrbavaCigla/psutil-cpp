cmake_minimum_required(VERSION "3.4.0")

project("psutil-cpp" VERSION 0.0.1 DESCRIPTION "psutil ")


set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_LOWERCASE)

add_library("${PROJECT_NAME}"
        "src/${PROJECT_NAME}/utils.cpp"
        "src/${PROJECT_NAME}/utils.hpp"
        "src/${PROJECT_NAME}/${SYSTEM_NAME_LOWERCASE}/psutil.cpp"
        "src/${PROJECT_NAME}/${SYSTEM_NAME_LOWERCASE}/psutil.hpp"
)

target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/${SYSTEM_NAME_LOWERCASE}")

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
