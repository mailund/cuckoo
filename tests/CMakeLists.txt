include(CTest)

include_directories("${PROJECT_SOURCE_DIR}/libcuckoo")

add_executable(creation_test creation_test.c)
target_link_libraries(creation_test cuckoo)
add_test(CreationTest creation_test)

