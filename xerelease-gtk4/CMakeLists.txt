set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.0.0)
project(xerelease-gtk4 VERSION 10.0.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
include_directories(.)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK4 REQUIRED gtk4)
include_directories (${GTK4_INCLUDE_DIRS})
link_directories (${GTK4_LIBRARY_DIRS})
add_executable(xerelease-gtk4 src/main.cpp src/gtkwin.cpp 
src/xeapi.cpp src/xerelease.cpp src/resources.cpp)
add_definitions (${GTK4_CFLAGS_OTHER})
target_link_libraries (${PROJECT_NAME} ${GTK4_LIBRARIES})
