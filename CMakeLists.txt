cmake_minimum_required(VERSION 3.8)
project(SF)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(ThreadPool)
add_subdirectory(SF)
add_subdirectory(tests)
