cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(yavin)
# project(yavin VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for YCM
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-fcolor-diagnostics")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always")
endif()

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

file(GLOB src Yavin/*.cpp)

# create library target
add_library(yavin SHARED ${src})

# Define headers for this library.
target_include_directories(yavin PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

# try to find required packages
find_package(gl3w REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL)
find_package(PNG REQUIRED)
find_package(PNG++ REQUIRED)

set(GL3W_INCLUDE_DIR ${gl3w_DIR}/../include/)

target_include_directories(yavin PUBLIC 
  ${OPENGL_INCLUDE_DIR} 
  ${GLM_INCLUDE_DIRS} 
  ${GLFW3_INCLUDE_DIRECTORIES}
  ${GL3W_INCLUDE_DIR})

target_link_libraries(yavin 
  ${OPENGL_gl_LIBRARY} 
  ${OPENGL_glu_LIBRARY} 
  gl3w 
  ${GLFW_LIBRARIES} 
  ${PNG_LIBRARIES} 
  ${PNG+_LIBRARIES}
  )

# TODO this must be somehow exported
link_directories(${PMP_BUILD_PATH})

# This makes the project importable from the build directory
export(TARGETS yavin FILE tatooineConfig.cmake)

# configure include files
configure_file(Yavin/config.h.in "${CMAKE_CURRENT_BINARY_DIR}/Yavin/config.h" @ONLY)

add_subdirectory(test)
