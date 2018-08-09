cmake_minimum_required(VERSION 3.10)
project(CuckooHash)
include(CTest)

add_subdirectory(libcuckoo) 
add_subdirectory(tests) 
