add_executable(dev_test main.cpp)
target_link_libraries(dev_test xVTF)
target_include_directories(dev_test PUBLIC ${PROJECT_SOURCE_DIR}/xVTF/include)