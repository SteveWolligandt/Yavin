cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(yavin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


set(YAVIN_SOURCES
  ${CMAKE_SOURCE_DIR}/src/atomic_counter_buffer.cpp
  ${CMAKE_SOURCE_DIR}/src/attribute.cpp
  ${CMAKE_SOURCE_DIR}/src/compute_shader.cpp
  ${CMAKE_SOURCE_DIR}/src/context.cpp
  ${CMAKE_SOURCE_DIR}/src/error_check.cpp
  ${CMAKE_SOURCE_DIR}/src/fragment_shader.cpp
  ${CMAKE_SOURCE_DIR}/src/frame_buffer.cpp
  ${CMAKE_SOURCE_DIR}/src/geometry_shader.cpp
  ${CMAKE_SOURCE_DIR}/src/gl_functions.cpp
  ${CMAKE_SOURCE_DIR}/src/glsl_var.cpp
  ${CMAKE_SOURCE_DIR}/src/gl_wrapper.cpp
  ${CMAKE_SOURCE_DIR}/src/index_buffer.cpp
  ${CMAKE_SOURCE_DIR}/src/mutex_handler.cpp
  ${CMAKE_SOURCE_DIR}/src/oit_window.cpp
  ${CMAKE_SOURCE_DIR}/src/orthographic_camera.cpp
  ${CMAKE_SOURCE_DIR}/src/perspective_camera.cpp
  ${CMAKE_SOURCE_DIR}/src/shader.cpp
  ${CMAKE_SOURCE_DIR}/src/shader_stage.cpp
  ${CMAKE_SOURCE_DIR}/src/shader_stage_parser.cpp
  ${CMAKE_SOURCE_DIR}/src/tesselation_control_shader.cpp
  ${CMAKE_SOURCE_DIR}/src/tesselation_evaluation_shader.cpp
  ${CMAKE_SOURCE_DIR}/src/texture.cpp
  ${CMAKE_SOURCE_DIR}/src/vertex_array.cpp
  ${CMAKE_SOURCE_DIR}/src/vertex_shader.cpp
  ${CMAKE_SOURCE_DIR}/src/window.cpp
  ${CMAKE_BINARY_DIR}/src/gl_verbose.cpp
)

# create library target
add_library(yavin SHARED ${YAVIN_SOURCES})
target_compile_features(yavin PUBLIC cxx_std_17)

# for color with ninja
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options(-fcolor-diagnostics)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
endif()

target_compile_definitions(yavin
  PRIVATE
    DLL_EXPORT)

target_compile_options(yavin
  PRIVATE
    -Wall
    -Wextra
    -Wno-missing-braces
    -pedantic
)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL)
find_path(GLEW_INCLUDE_DIR GL/glew.h)
find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s PATH_SUFFIXES lib64)
find_package(GLFW3)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(PNG)
find_package(PNG++)
find_package(Boost REQUIRED)
find_package(X11 REQUIRED)

if(GLFW3_FOUND)
  target_compile_definitions(yavin
    PRIVATE
      USE_GLFW3)
endif()

if(PNG++_FOUND)
  target_compile_definitions(yavin
    PRIVATE
    USE_PNG)
endif()

option(VERBOSE "verbose OpenGL calls" OFF)
if (VERBOSE)
  set(VERBOSE_GL "true")
else()
  set(VERBOSE_GL "false")
endif()
configure_file(src/gl_verbose.cpp.in src/gl_verbose.cpp)
configure_file(include/yavin/shader_include_paths.h.in include/yavin/shader_include_paths.h)

target_include_directories(yavin
  PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/single_include
    ${CMAKE_BINARY_DIR}/include
    ${OPENGL_INCLUDE_DIR} 
    ${GLM_INCLUDE_DIRS} 
    ${GLFW3_INCLUDE_DIRECTORIES}
    ${GLEW_INCLUDE_DIR}
    ${X11_INCLUDE_DIR}
)

target_link_libraries(yavin 
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARY}
  ${GLFW_LIBRARIES} 
  ${PNG_LIBRARIES} 
  ${PNG+_LIBRARIES}
  ${X11_LIBRARIES}
)

export(TARGETS yavin FILE yavinConfig.cmake)

add_subdirectory(test)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
  # first we can indicate the documentation build as an option and set it to ON by default
  option(BUILD_DOC "Generate documentation" OFF)
  if (BUILD_DOC)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(doc_doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM)
  endif()
endif ()
