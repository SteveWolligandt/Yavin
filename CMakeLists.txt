cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(yavin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
set(GLEW_BUILD "GLX" CACHE STRING "GLX or EGL")
set_property(CACHE GLEW_BUILD PROPERTY STRINGS GLX EGL)

if(GLEW_BUILD STREQUAL "GLX")
  set(USE_EGL 0)
  set(USE_GLX 1)
elseif(GLEW_BUILD STREQUAL "EGL")
  set(USE_EGL 1)
  set(USE_GLX 0)
endif()
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
  #- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  # build glew
  execute_process(COMMAND make
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/glew/auto)
  if(USE_GLX)
    execute_process(COMMAND make
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/glew
                    RESULT_VARIABLE GLEW_BUILD_RESULT)
  elseif(USE_EGL)
    execute_process(COMMAND make SYSTEM=linux-egl
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/glew
                    RESULT_VARIABLE GLEW_BUILD_RESULT)
  endif()
  if(NOT GLEW_BUILD_RESULT EQUAL 0)
    message(FATAL_ERROR "building GLEW failed with ${GLEW_BUILD_RESULT}!")
  endif()
endif()
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# for color with ninja
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options(-fcolor-diagnostics)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
endif()
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
set(YAVIN_SOURCES
  ${CMAKE_SOURCE_DIR}/src/atomiccounterbuffer.cpp
  ${CMAKE_SOURCE_DIR}/src/attribute.cpp
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
if(USE_GLX)
  list(APPEND YAVIN_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/glx_context.cpp
    ${CMAKE_SOURCE_DIR}/src/glx_window.cpp
  )
elseif(USE_EGL)
  list(APPEND YAVIN_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/egl_context.cpp
    ${CMAKE_SOURCE_DIR}/src/egl_window.cpp
  )
endif()

# create library target
add_library(yavin SHARED ${YAVIN_SOURCES} ${YAVIN_GLFW_SOURCES})
target_compile_features(yavin PUBLIC cxx_std_17)

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
set(GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glew/include)
set(GLEW_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/external/glew/lib/libGLEW.so)
find_package(glm REQUIRED)
find_package(Boost REQUIRED)
if(USE_GLX)
  find_package(OpenGL REQUIRED COMPONENTS OpenGL GLX)
  find_package(X11 REQUIRED)
elseif(USE_EGL)
  find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL)
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/single_include/yavin.in"
               "${CMAKE_CURRENT_BINARY_DIR}/single_include/yavin")


target_include_directories(yavin
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/single_include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${OPENGL_INCLUDE_DIR} 
    ${GLM_INCLUDE_DIRS} 
    ${GLEW_INCLUDE_DIR}
)

target_link_libraries(yavin 
 PRIVATE
  OpenGL::GL
  OpenGL::GLU
  ${GLEW_LIBRARY}
)

if(USE_EGL)
  target_link_libraries(yavin PRIVATE OpenGL::EGL)
elseif(USE_GLX)
  target_link_libraries(yavin PRIVATE OpenGL::GLX ${X11_LIBRARIES})
  target_include_directories(yavin PUBLIC ${X11_INCLUDE_DIR})
endif()
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
find_package(PNG)
find_package(PNG++)
if(PNG_FOUND AND PNG++_FOUND)
  target_include_directories(yavin PUBLIC
                             ${PNG_INCLUDE_DIRS} ${PNG++_INCLUDE_DIRS})
  target_link_libraries(yavin
                         PRIVATE
                          ${PNG_LIBRARIES} ${PNG++_LIBRARIES})
  set(USE_PNG "true")
else()
  set(USE_PNG "false")
endif()

option(VERBOSE "verbose OpenGL calls" OFF)
if (VERBOSE)
  set(VERBOSE_GL "true")
else()
  set(VERBOSE_GL "false")
endif()

configure_file(src/glverbose.cpp.in
               src/glverbose.cpp)
configure_file(include/yavin/shaderincludepaths.h.in
               include/yavin/shaderincludepaths.h)
configure_file(include/yavin/texture.h.in
               include/yavin/texture.h)
configure_file(include/yavin/settings.h.in
               include/yavin/settings.h)

export(TARGETS yavin FILE yavinConfig.cmake)
export(PACKAGE yavin)

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
