# Configs
list(APPEND CFGHEADERS
cmake_file.h
)

set(DATASETDIR ${PROJECT_SOURCE_DIR}/tests/data)

file(REMOVE .gitignore)
file(APPEND .gitignore "# CMake Generated Headers\n\n")

foreach(x IN LISTS CFGHEADERS)
	set(y)
	string(REGEX REPLACE "cmake_*" "" y ${x})
	file(APPEND .gitignore ${y}\n)
	configure_file(${x} ${PROJECT_SOURCE_DIR}/tests/${y})
endforeach()

set(interface ${PROJECT_SOURCE_DIR}/xVTF/include)

add_executable(test_cpp_linkage cpp_linkage.cpp)
target_link_libraries(test_cpp_linkage xVTF)
target_include_directories(test_cpp_linkage PUBLIC ${interface})

add_executable(test_cpp_loading cpp_loading.cpp cmake_file.h assert_equal.h library_error.h)
target_link_libraries(test_cpp_loading xVTF)
target_include_directories(test_cpp_loading PUBLIC ${interface})

set_target_properties(test_cpp_loading test_cpp_linkage PROPERTIES FOLDER Tests)
