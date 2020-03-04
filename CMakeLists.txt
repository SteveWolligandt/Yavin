cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(yavin)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/catch2/contrib/")
add_subdirectory(external/catch2)
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# for color with ninja
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options(-fcolor-diagnostics)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
endif()
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# git submodules
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  # Update submodules as needed
  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  RESULT_VARIABLE GIT_SUBMOD_RESULT)
  if(NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
  endif()
endif()

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# build glew
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
set(YAVIN_CONTEXT_SYSTEM "GLX" CACHE STRING "GLX or EGL")
set_property(CACHE YAVIN_CONTEXT_SYSTEM PROPERTY STRINGS GLX EGL)
set(YAVIN_GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glew)
set(YAVIN_GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glew/include)
set(YAVIN_GLEW_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/external/glew/lib/libGLEW.a)

if(YAVIN_CONTEXT_SYSTEM STREQUAL "GLX")
  set(YAVIN_USE_EGL 0)
  set(YAVIN_USE_GLX 1)
elseif(YAVIN_CONTEXT_SYSTEM STREQUAL "EGL")
  set(YAVIN_USE_EGL 1)
  set(YAVIN_USE_GLX 0)
endif()
add_custom_target(glew.download
                  COMMAND make
                  WORKING_DIRECTORY ${YAVIN_GLEW_DIR}/auto)
add_custom_target(glew.build.clean
                  COMMAND make clean
                  WORKING_DIRECTORY ${YAVIN_GLEW_DIR})
add_custom_target(glew.download.clean
                  COMMAND make clean
                  WORKING_DIRECTORY ${YAVIN_GLEW_DIR}/auto)
add_custom_target(glew.clean
                  DEPENDS glew.download.clean glew.build.clean
                  WORKING_DIRECTORY ${YAVIN_GLEW_DIR})
if(YAVIN_USE_GLX)
  add_custom_command(OUTPUT ${YAVIN_GLEW_LIBRARY}
                    COMMAND make
                    DEPENDS glew.download
                    WORKING_DIRECTORY ${YAVIN_GLEW_DIR})
elseif(YAVIN_USE_EGL)
  add_custom_command(OUTPUT ${YAVIN_GLEW_LIBRARY}
                    COMMAND make SYSTEM=linux-egl
                    DEPENDS glew.download
                    WORKING_DIRECTORY ${YAVIN_GLEW_DIR})
endif()
add_custom_target(glew.build
                  DEPENDS ${YAVIN_GLEW_LIBRARY}
                  WORKING_DIRECTORY ${YAVIN_GLEW_DIR})

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# find OpenGL packages
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
set(OpenGL_GL_PREFERENCE "GLVND")
if(YAVIN_USE_GLX)
  find_package(OpenGL REQUIRED COMPONENTS OpenGL GLX)
  find_package(X11 REQUIRED)
elseif(YAVIN_USE_EGL)
  find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL)
endif()

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# find misc packages
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
find_package(Boost REQUIRED)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# setup yavin library target
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
set(YAVIN_SOURCES
  ${CMAKE_SOURCE_DIR}/src/atomiccounterbuffer.cpp
  ${CMAKE_SOURCE_DIR}/src/computeshader.cpp
  ${CMAKE_SOURCE_DIR}/src/errorcheck.cpp
  ${CMAKE_SOURCE_DIR}/src/fragmentshader.cpp
  ${CMAKE_SOURCE_DIR}/src/framebuffer.cpp
  ${CMAKE_SOURCE_DIR}/src/geometryshader.cpp
  ${CMAKE_SOURCE_DIR}/src/glfunctions.cpp
  ${CMAKE_SOURCE_DIR}/src/glslvar.cpp
  ${CMAKE_SOURCE_DIR}/src/glwrapper.cpp
  ${CMAKE_SOURCE_DIR}/src/indexbuffer.cpp
  ${CMAKE_SOURCE_DIR}/src/mutexhandler.cpp
  ${CMAKE_SOURCE_DIR}/src/orthographiccamera.cpp
  ${CMAKE_SOURCE_DIR}/src/perspectivecamera.cpp
  ${CMAKE_SOURCE_DIR}/src/shader.cpp
  ${CMAKE_SOURCE_DIR}/src/shaderstage.cpp
  ${CMAKE_SOURCE_DIR}/src/shaderstageparser.cpp
  ${CMAKE_SOURCE_DIR}/src/tesselationcontrolshader.cpp
  ${CMAKE_SOURCE_DIR}/src/tesselationevaluationshader.cpp
  ${CMAKE_SOURCE_DIR}/src/texture.cpp
  ${CMAKE_SOURCE_DIR}/src/vertexarray.cpp
  ${CMAKE_SOURCE_DIR}/src/vertexshader.cpp
  ${CMAKE_BINARY_DIR}/src/glverbose.cpp
)
if(YAVIN_USE_GLX)
  list(APPEND YAVIN_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/glx_context.cpp
    ${CMAKE_SOURCE_DIR}/src/glx_window.cpp
  )
elseif(YAVIN_USE_EGL)
  list(APPEND YAVIN_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/egl_context.cpp
    ${CMAKE_SOURCE_DIR}/src/egl_window.cpp
  )
endif()
add_library(yavin SHARED ${YAVIN_GLEW_LIBRARY} ${YAVIN_SOURCES})
target_compile_features(yavin PUBLIC cxx_std_17)

target_compile_definitions(yavin PRIVATE DLL_EXPORT)
target_compile_options(yavin PRIVATE
                       -Wall -Wextra -Wno-missing-braces -pedantic)

target_include_directories(yavin
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/single_include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/single_include
    ${OPENGL_INCLUDE_DIR} 
    ${YAVIN_GLEW_INCLUDE_DIR}
)

target_link_libraries(
  yavin PRIVATE
  OpenGL::OpenGL OpenGL::GLU ${YAVIN_GLEW_LIBRARY})
if(YAVIN_USE_GLX)
  target_link_libraries(
    yavin PRIVATE
    OpenGL::GLX ${X11_LIBRARIES})
  target_include_directories(
    yavin PRIVATE
    ${X11_INCLUDE_DIR})
elseif(YAVIN_USE_EGL)
  target_link_libraries(
    yavin PRIVATE
    OpenGL::EGL)
endif()

# target export
export(TARGETS yavin FILE yavinConfig.cmake)
export(PACKAGE yavin)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# setup png library
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
find_package(PNG)
find_package(PNG++)
if(PNG_FOUND AND PNG++_FOUND)
  target_include_directories(
    yavin PRIVATE
    ${PNG_INCLUDE_DIRS} ${PNG++_INCLUDE_DIRS})
  target_link_libraries(
    yavin PRIVATE
    ${PNG_LIBRARIES} ${PNG++_LIBRARIES})
  set(YAVIN_HAS_PNG_SUPPORT true)
else()
  set(YAVIN_HAS_PNG_SUPPORT false)
endif()

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# OpenGL call output
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
option(VERBOSE "verbose OpenGL calls" OFF)
if (VERBOSE)
  set(VERBOSE_GL "true")
else()
  set(VERBOSE_GL "false")
endif()

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# configure files
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/yavin/context.h.in"
               "${CMAKE_CURRENT_BINARY_DIR}/include/yavin/context.h")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/yavin/window.h.in"
               "${CMAKE_CURRENT_BINARY_DIR}/include/yavin/window.h")
configure_file(src/glverbose.cpp.in
               src/glverbose.cpp)
configure_file(include/yavin/shaderincludepaths.h.in
               include/yavin/shaderincludepaths.h)
configure_file(include/yavin/texpng.h.in
               include/yavin/texpng.h)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# subdirectories
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
add_subdirectory(test)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# Doxygen
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
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
