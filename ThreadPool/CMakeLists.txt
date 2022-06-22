cmake_minimum_required (VERSION 3.8)

add_library(ThreadPool STATIC "Headers/ThreadPool.h" "Headers/SafeQueue.h" "Sources/ThreadPool.cpp")
