add_executable(DevTest main.cpp)
target_link_libraries(DevTest xVTF)
target_include_directories(DevTest PUBLIC ${PROJECT_SOURCE_DIR}/xVTF/include)