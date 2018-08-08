include(CTest)

include_directories("${PROJECT_SOURCE_DIR}/libcuckoo")
set(PROJECT_LIBS cuckoo)

file(GLOB files "*.c")
foreach(file ${files})
	string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${file})
	add_executable(${file_without_ext} ${file})
	target_link_libraries(${file_without_ext} ${PROJECT_LIBS})
	add_test(${file_without_ext} ${file_without_ext})
endforeach()

