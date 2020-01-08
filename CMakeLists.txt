cmake_minimum_required(VERSION 2.8)

project(SGA_TEST)
set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. DIR_MAIN)

include_directories("${PROJECT_SOURCE_DIR}/src")
add_subdirectory(src)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build) 
add_executable(SGA_TEST ${PROJECT_SOURCE_DIR}/test/test.cpp)

link_directories("${PROJECT_SOURCE_DIR}/lib")
target_link_libraries(SGA_TEST sgalib)
