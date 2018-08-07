include(CTest)

include_directories ("${PROJECT_SOURCE_DIR}/libcuckoo")

add_executable(creation_test creation_test.c)
add_test(CreationTest creation_test)

