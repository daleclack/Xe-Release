set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.0.0)
project(XeRelease VERSION 12.1.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
include_directories(..)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTKMM4 REQUIRED gtkmm-4.0)
include_directories (${GTKMM4_INCLUDE_DIRS})
link_directories (${GTKMM4_LIBRARY_DIRS})

if(WIN32)
    SET(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
	set(app_WINRC icon.rc)
	set_property(SOURCE icon.rc APPEND PROPERTY
        OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/icon.ico
        )
	add_executable(XeRelease WIN32 ${app_WINRC} src/main.cc src/MyWin.cc src/MyDialog.cc 
    src/xerelease.cc src/xeapi.cc src/resources.cpp)
else()
    add_executable(XeRelease src/main.cc src/MyWin.cc src/MyDialog.cc 
    src/xerelease.cc src/xeapi.cc src/resources.cpp)
endif(WIN32)

SET (CMAKE_EXTRA_CXX_FLAGS ${GTKMM4_CFLAGS_OTHER})
target_link_libraries (${PROJECT_NAME} ${GTKMM4_LIBRARIES})