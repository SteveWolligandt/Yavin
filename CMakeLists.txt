cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(yavin)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/catch2/contrib/")
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
set(YAVIN_GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glew)
set(YAVIN_GLEW_INCLUDE_DIR ${YAVIN_GLEW_DIR}/include)
set(YAVIN_GLEW_LIBRARY ${YAVIN_GLEW_DIR}/lib/libGLEW.a)

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
add_custom_command(OUTPUT ${YAVIN_GLEW_LIBRARY}
                  COMMAND make SYSTEM=linux-egl
                  DEPENDS glew.download
                  WORKING_DIRECTORY ${YAVIN_GLEW_DIR})
add_custom_target(glew.build
                  DEPENDS ${YAVIN_GLEW_LIBRARY}
                  WORKING_DIRECTORY ${YAVIN_GLEW_DIR})

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# build imgui
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
set(YAVIN_IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
set(YAVIN_IMGUI_INCLUDE_DIRS ${YAVIN_IMGUI_DIR}/.. ${YAVIN_IMGUI_DIR})
file(GLOB YAVIN_IMGUI_FILES "${YAVIN_IMGUI_DIR}/*.cpp")

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# find OpenGL packages
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# find misc packages
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
find_package(X11 REQUIRED)
find_package(Boost REQUIRED)
message(STATUS ${X11_LIBRARIES})

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# clang-tidy
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
set(YAVIN_COMPILE_WITH_CLANG_TIDY_CHECKS false CACHE BOOL
    "use clang-tidy checks while building")
option(YAVIN_COMPILE_WITH_CLANG_TIDY_CHECKS
       "use clang-tidy checks while building" FALSE)
find_program(YAVIN_CLANG_TIDY NAMES "clang-tidy" DOC "Path to clang-tidy executable")
string(CONCAT YAVIN_CLANG_TIDY_CHECKS
  "-checks="
  ",cppcoreguidelines*"
  ",-cppcoreguidelines-pro-type-reinterpret-cast"
  ",-cppcoreguidelines-pro-bounds-pointer-arithmetic"
  ",-cppcoreguidelines-pro-bounds-constant-array-index"
  ",-cppcoreguidelines-avoid-magic-numbers"
  ",-cppcoreguidelines-macro-usage"
  ",cert*"
  ",-cert-err34-c"
  ",-cert-dcl21-cpp"
  ",-cert-oop54-cpp" #self-assignment
  ",hicpp"
  ",modernize*"
  ",performance*"
  ",readability*"
  ",-readability-magic-numbers"
  ",-readability-isolate-declaration"
  ",-readability-static-accessed-through-instance"
  ",-readability-named-parameter"
  ",google-explicit-constructor"
  )
if(YAVIN_CLANG_TIDY)
  set(YAVIN_CLANG_TIDY_CMD
    ${YAVIN_CLANG_TIDY}
    ${YAVIN_CLANG_TIDY_CHECKS}
    "-header-filter=yavin")
endif()

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
  ${CMAKE_SOURCE_DIR}/src/window_notifier.cpp
  ${CMAKE_SOURCE_DIR}/src/keyboard.cpp
  ${CMAKE_SOURCE_DIR}/src/mouse.cpp
  ${CMAKE_SOURCE_DIR}/src/imgui_api_backend.cpp
  ${CMAKE_SOURCE_DIR}/src/imgui_shader.cpp
  ${CMAKE_SOURCE_DIR}/src/imgui_render_backend.cpp
  ${CMAKE_SOURCE_DIR}/src/imgui.cpp
  ${CMAKE_SOURCE_DIR}/src/egl/error_string.cpp
  ${CMAKE_SOURCE_DIR}/src/egl/context.cpp
  ${CMAKE_SOURCE_DIR}/src/egl/surface.cpp
  ${CMAKE_SOURCE_DIR}/src/egl/display.cpp
  ${CMAKE_SOURCE_DIR}/src/X11/display.cpp
  ${CMAKE_SOURCE_DIR}/src/X11/window.cpp
  ${CMAKE_BINARY_DIR}/src/glverbose.cpp
  ${CMAKE_SOURCE_DIR}/src/context.cpp
  ${CMAKE_SOURCE_DIR}/src/window.cpp
)
add_library(yavin SHARED ${YAVIN_GLEW_LIBRARY} ${YAVIN_SOURCES} ${YAVIN_IMGUI_FILES})
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
    ${YAVIN_IMGUI_INCLUDE_DIRS}
    ${X11_INCLUDE_DIR})

target_link_libraries(yavin
                       PRIVATE
                        OpenGL::OpenGL
                        OpenGL::GLU
                        ${YAVIN_GLEW_LIBRARY}
                        ${X11_LIBRARIES})
target_link_libraries(yavin PRIVATE OpenGL::EGL)

if(YAVIN_CLANG_TIDY AND YAVIN_COMPILE_WITH_CLANG_TIDY_CHECKS)
  set_target_properties(yavin PROPERTIES CXX_CLANG_TIDY "${YAVIN_CLANG_TIDY_CMD}")
endif()

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
option(YAVIN_VERBOSE "verbose OpenGL calls" OFF)
if (YAVIN_VERBOSE)
  set(VERBOSE_GL "true")
else()
  set(VERBOSE_GL "false")
endif()

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# configure files
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
configure_file(src/glverbose.cpp.in
               src/glverbose.cpp)
configure_file(include/yavin/shaderincludepaths.h.in
               include/yavin/shaderincludepaths.h)
configure_file(include/yavin/texpng.h.in
               include/yavin/texpng.h)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# subdirectories
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
add_subdirectory(external/catch2)
add_subdirectory(test)
add_subdirectory(doc)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# target export
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
export(TARGETS yavin FILE yavinConfig.cmake)
export(PACKAGE yavin)
