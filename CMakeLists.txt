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

add_library(${PROJECT_NAME} SHARED ${LIBSRC})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)


install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib        
)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)


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
