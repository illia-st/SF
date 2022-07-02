cmake_minimum_required (VERSION 3.8)

add_executable(Tests main.cpp)

target_link_libraries(Tests SF)
target_link_libraries(Tests ThreadPool)
include_directories(../SF)
include_directories(../ThreadPool)