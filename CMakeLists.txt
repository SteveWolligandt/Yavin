cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(yavin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for YCM
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-fcolor-diagnostics")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always")
endif()

# try to find required packages
find_path(GLEW_INCLUDE_DIR GL/glew.h)
find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s PATH_SUFFIXES lib64)
find_package(gl3w REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL)
# find_package(PNG REQUIRED)
# find_package(PNG++ REQUIRED)

set(GL3W_INCLUDE_DIR ${gl3w_DIR}/../include/)
set(GL3W_SOURCE_DIR ${gl3w_DIR}/../src/)
set(GL3W_SOURCES ${GL3W_SOURCE_DIR}/gl3w.c)

file(GLOB YAVIN_SOURCES yavin_include/*.cpp)
file(GLOB YAVIN_HEADERS yavin_include/*.h)

# create library target
add_library(yavin SHARED ${YAVIN_SOURCES} ${YAVIN_HEADERS})
target_compile_definitions(yavin PRIVATE DLL_EXPORT)
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(yavin PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${OPENGL_INCLUDE_DIR} 
  ${GLM_INCLUDE_DIRS} 
  ${GLFW3_INCLUDE_DIRECTORIES}
  ${GL3W_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIR}
  )

target_link_libraries(yavin 
  ${OPENGL_gl_LIBRARY} 
  ${OPENGL_glu_LIBRARY}
  ${GLFW_LIBRARIES} 
  ${PNG_LIBRARIES} 
  ${PNG+_LIBRARIES}
  ${GLEW_LIBRARY}
  )

# This makes the project importable from the build directory
export(TARGETS yavin FILE yavinConfig.cmake)

add_subdirectory(test)
