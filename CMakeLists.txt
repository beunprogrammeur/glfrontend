# default setup
cmake_minimum_required(VERSION 2.8.9)
project (glfrontend)

# needed for gdb
set(CMAKE_BUILD_TYPE Debug)

# included source is found in the ./incude directory
include_directories(./include)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE INCL_SOURCES "include/*.cpp")


find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)






# report a solution if OpenGL is not found
if(NOT OPENGL_FOUND)
	message("OpenGL dev not found, install libgl1-mesa-dev first")
endif()


add_executable (glfrontend ${SOURCES} ${INCL_SOURCES})

target_include_directories(glfrontend PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(glfrontend glfw ${OPENGL_gl_LIBRARY})
