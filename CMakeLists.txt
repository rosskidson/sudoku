cmake_minimum_required (VERSION 2.6)
project (euler_project_questions)
add_definitions(-std=c++0x)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(solver src/main.cpp)
