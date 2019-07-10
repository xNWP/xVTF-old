# xVTF Library

# Configs
list(APPEND CFGHEADERS
cmakeheaders/cmake_exports.h
)

file(REMOVE include/xVTF/.gitignore)
file(APPEND include/xVTF/.gitignore "# CMake Generated Headers\n\n")

foreach(x IN LISTS CFGHEADERS)
	set(y)
	string(REGEX REPLACE "cmakeheaders/cmake_*" "" y ${x})
	file(APPEND include/xVTF/.gitignore ${y}\n)
	list(APPEND HEADERS include/xVTF/${y})
	configure_file(${x} ${PROJECT_SOURCE_DIR}/xVTF/include/xVTF/${y})
endforeach()

set(SOURCES
src/bitmapimage.cpp
src/codecs.cpp
src/codecs.h
src/error_macro.h
src/luts.h
src/vtf_file.cpp
src/vtfheader.h
)

list(APPEND HEADERS
include/xVTF/bitmapimage.h
include/xVTF/error.h
include/xVTF/exports.h
include/xVTF/flags.h
include/xVTF/pixelformats.h
include/xVTF/types.h
include/xVTF/vtf_file.h
include/xVTF/xVTF.h
)

SOURCE_GROUP(src FILES ${SOURCES})
SOURCE_GROUP(include FILES ${HEADERS})
SOURCE_GROUP(CMakeHeaders FILES ${CFGHEADERS})

# Binary
add_library(xVTF SHARED ${SOURCES} ${HEADERS} ${CFGHEADERS})
add_library(xVTFs STATIC ${SOURCES} ${HEADERS} ${CFGHEADERS})
target_include_directories(xVTF PUBLIC include)
target_include_directories(xVTFs PUBLIC include)
target_compile_definitions(xVTF PRIVATE XVTF_EXPORTS)
target_compile_definitions(xVTFs PUBLIC XVTF_STATIC)

set_property(TARGET xVTF xVTFs PROPERTY DEBUG_POSTFIX _d)