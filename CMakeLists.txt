# default setup
cmake_minimum_required(VERSION 2.8.9)
project (glfrontend)

# needed for gdb
set(CMAKE_BUILD_TYPE Debug)

# included source is found in the ./incude directory
include_directories(./include)
include_directories(./src)
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE INCL_SOURCES "include/*.cpp")

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW required glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

# defines passed to the compiler

add_definitions(-DWINDOW_TITLE="GLFrontend")
add_definitions(-DDEFAULT_SCREEN_HEIGHT=600)
add_definitions(-DDEFAULT_SCREEN_WIDTH=800)
add_definitions(-DIMAGE_SHADER_VERTEX_FILE="./data/shaders/image.vert")
add_definitions(-DIMAGE_SHADER_FRAGMENT_FILE="./data/shaders/image.frag")
add_definitions(-DSETTINGS_PATH="./data/settings.ini")

# report a solution if OpenGL is not found
if(NOT OPENGL_FOUND)
    message("OpenGL dev not found, install libgl1-mesa-dev first")
endif()





add_executable (glfrontend ${SOURCES} ${INCL_SOURCES})

target_include_directories(glfrontend PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(glfrontend glfw3 GL GLU glfw3 X11 Xxf86vm Xrandr pthread Xi dl Xcursor Xinerama rt m sqlite3 ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES})


# copy data folder to build directory

add_custom_command(
        TARGET glfrontend PRE_BUILD
        COMMAND cp -ru ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/.
        COMMENT "copied/updated 'data' dir"
)

