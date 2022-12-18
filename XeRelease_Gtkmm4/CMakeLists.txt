set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.0.0)
project(XeRelease VERSION 15.0.0)

#Add GCR_CMake
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../GCR_CMake/macros)
include(GlibCompileResourcesSupport)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
include_directories(..)
include_directories(../cfgfile2)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTKMM4 REQUIRED gtkmm-4.0)
include_directories (${GTKMM4_INCLUDE_DIRS})
link_directories (${GTKMM4_LIBRARY_DIRS})

set(SOURCES src/main.cc src/MyWin.cc src/MyPrefs.cc
    src/xerelease.cc src/xeapi.cc)

#Compile resources with GCR_CMake
set(RESOURCE_LIST
    style.css
    style_dark.css
    STRIPBLANKS menubar.xml
    STRIPBLANKS prefs.ui
    icons/16x16/actions/open-menu.png
    icons/48x48/actions/Xe-Release.png)

compile_gresources(RESOURCE_FILE
                   XML_OUT
                   TYPE EMBED_C
                   RESOURCES ${RESOURCE_LIST}
                   PREFIX "/org/gtk/daleclack"
                   SOURCE_DIR ${PROJECT_SOURCE_DIR}/res)

# Add a custom target to the makefile. Now make builds our resource file.
# It depends on the output RESOURCE_FILE.
add_custom_target(resource ALL DEPENDS ${RESOURCE_FILE})

if(WIN32)
    SET(CMAKE_RC_COMPILER windres)
	set(app_WINRC icon.rc)
	set_property(SOURCE icon.rc APPEND PROPERTY
        OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/icon.ico
        )
	add_executable(XeRelease WIN32 ${app_WINRC} ${SOURCES} ${RESOURCE_FILE})
else()
    add_executable(XeRelease ${SOURCES} ${RESOURCE_FILE})
endif(WIN32)

SET (CMAKE_EXTRA_CXX_FLAGS ${GTKMM4_CFLAGS_OTHER})
target_link_libraries (${PROJECT_NAME} ${GTKMM4_LIBRARIES})
