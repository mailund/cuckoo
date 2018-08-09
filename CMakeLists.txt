cmake_minimum_required(VERSION 3.10)
project(CuckooHash)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
include(CTest)

option(COVERALLS "Generate coveralls data" OFF)
if (COVERALLS)
    include(Coveralls)
    coveralls_turn_on_coverage()
    #FIXME: There must be a better way to set these files
    set(COVERAGE_SRCS 
    	libcuckoo/cuckoo.h 
    	libcuckoo/cuckoo_internal.h 
    	libcuckoo/cuckoo.c)
    coveralls_setup("${COVERAGE_SRCS}" ON) # ON / OFF upload
endif()


add_subdirectory(libcuckoo) 
add_subdirectory(tests) 
