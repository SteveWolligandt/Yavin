cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(yavin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# create library target
add_library(yavin SHARED ${YAVIN_SOURCES} ${CMAKE_BINARY_DIR}/yavin_include/gl_verbose.cpp ${YAVIN_HEADERS})
target_compile_features(yavin PUBLIC cxx_std_17)
target_compile_definitions(yavin PRIVATE DLL_EXPORT)
target_include_directories(yavin PRIVATE 
  ${CMAKE_BINARY_DIR}/yavin_include
)

# for color with ninja
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "-fcolor-diagnostics")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always")
endif()

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL)
find_path(GLEW_INCLUDE_DIR GL/glew.h)
find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s PATH_SUFFIXES lib64)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(PNG REQUIRED)
find_package(PNG++ REQUIRED)
find_package(Boost REQUIRED)

file(GLOB YAVIN_SOURCES yavin_include/*.cpp)
file(GLOB YAVIN_HEADERS yavin_include/*.h)

option(VERBOSE "verbose OpenGL calls" OFF)
if (VERBOSE)
  set(VERBOSE_GL "true")
else()
  set(VERBOSE_GL "false")
endif()
configure_file(yavin_include/gl_verbose.cpp.in yavin_include/gl_verbose.cpp)
configure_file(yavin_include/shader_include_paths.h.in yavin_include/shader_include_paths.h)

target_include_directories(yavin
  PUBLIC 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${OPENGL_INCLUDE_DIR} 
    ${GLM_INCLUDE_DIRS} 
    ${GLFW3_INCLUDE_DIRECTORIES}
    ${GLEW_INCLUDE_DIR}
)

target_link_libraries(yavin 
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARY}
  ${GLFW_LIBRARIES} 
  ${PNG_LIBRARIES} 
  ${PNG+_LIBRARIES}
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
