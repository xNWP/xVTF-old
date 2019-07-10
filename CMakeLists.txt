# CMakeLists.txt

cmake_minimum_required(VERSION 3.14)

project(xVTF	VERSION 100
				DESCRIPTION "A C/C++ library for opening/saving/manipulating Valve Texture Format (VTF) Images."
				LANGUAGES CXX)

set(XVTF_VERSION ${PROJECT_VERSION})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(xVTF)
add_subdirectory(dev_test)

# Standards
target_compile_features(xVTF PRIVATE
cxx_range_for
cxx_nullptr
cxx_lambdas
cxx_auto_type
cxx_rvalue_references
)

target_compile_features(xVTFs PRIVATE
cxx_range_for
cxx_nullptr
cxx_lambdas
cxx_auto_type
cxx_rvalue_references
)