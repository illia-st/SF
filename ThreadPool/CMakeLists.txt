cmake_minimum_required (VERSION 3.8)

add_library(ThreadPool STATIC
        BS_thread_pool.hpp
        BS_thread_pool.cpp)
