cmake_minimum_required(VERSION 3.4)

project(psutil-cpp DESCRIPTION "Psutil" VERSION "1.0")

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

option(BUILD_SHARED_LIBS OFF)
option(BUILD_TESTS OFF)
option(CMAKE_BUILD_TYPE Release)

string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_LOWER)

set(LIBSRC
    src/utils.cpp
    src/cpu.cpp
    src/mem.cpp
)

FILE(GLOB LIBHDRS ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/*)

add_library(${PROJECT_NAME} ${LIBSRC})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET ${PROJECT_NAME} PROPERTY PUBLIC_HEADER ${LIBHDRS})
target_include_directories(${PROJECT_NAME}
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:include>"
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/PsutilConfig.cmake"
    INSTALL_DESTINATION lib/cmake
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/PsutilConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY AnyNewerVersion
)

export(TARGETS ${PROJECT_NAME}
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/PsutilTargets.cmake"
)

include(GNUInstallDirs)
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/PsutilConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/PsutilConfigVersion.cmake"
    DESTINATION lib/cmake
)

install(TARGETS ${PROJECT_NAME} EXPORT Psutil)
install(
    EXPORT Psutil
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
    FILE "PsutilTargets.cmake"
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

if(BUILD_TESTS)
    add_executable(virtual_memory test/virtual_memory.cpp)
    add_executable(cpu_count      test/cpu_count.cpp)
    add_executable(cpu_times      test/cpu_times.cpp)
    add_executable(swap_memory    test/swap_memory.cpp)
    add_executable(cpu_freq       test/cpu_freq.cpp)

    target_link_libraries(virtual_memory ${PROJECT_NAME})
    target_link_libraries(cpu_count      ${PROJECT_NAME})
    target_link_libraries(swap_memory    ${PROJECT_NAME})
    target_link_libraries(cpu_times      ${PROJECT_NAME})
    target_link_libraries(cpu_freq       ${PROJECT_NAME})

    add_test("cpu_times"      cpu_times)
    add_test("virtual_memory" virtual_memory)
    add_test("swap_memory"    swap_memory)
    add_test("cpu_count"      cpu_count)
    add_test("cpu_freq"       cpu_freq)
endif()
