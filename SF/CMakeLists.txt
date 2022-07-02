cmake_minimum_required (VERSION 3.8)

add_library(SF STATIC "SF.h" "SF.cpp")

target_link_libraries(SF ThreadPool)

include_directories(../ThreadPool)


