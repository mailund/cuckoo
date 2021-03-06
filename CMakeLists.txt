cmake_minimum_required(VERSION 3.8)
project(CuckooHash)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
include(CTest)

option(COVERALLS "Generate coveralls data" OFF)
if (COVERALLS)
    include(Coveralls)
    coveralls_turn_on_coverage()
     set(COVERAGE_SRCS 
     	libcuckoo/cuckoo.h 
     	libcuckoo/cuckoo_internal.h 
     	libcuckoo/cuckoo.c)
     coveralls_setup("${COVERAGE_SRCS}" ON) # ON / OFF upload
 endif()
 
# Make sure we use C99 with gcc
if (CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endif()

add_subdirectory(libcuckoo) 
add_subdirectory(tests) 
