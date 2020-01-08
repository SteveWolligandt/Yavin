find_package(Catch2 REQUIRED)

enable_testing()
include(CTest)
include(Catch)

#-------------------------------------------------------------------------------
add_executable (egl_window egl_window.cpp)
target_compile_features(egl_window PRIVATE cxx_std_17)
target_link_libraries(egl_window yavin)
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
add_executable (glx_window glx_window.cpp)
target_compile_features(glx_window PRIVATE cxx_std_17)
target_link_libraries(glx_window yavin)
#-------------------------------------------------------------------------------
add_executable (unittests
  main.cpp
  tuple.cpp
  framebuffer.cpp
  indexbuffer.cpp
  vertexbuffer.cpp
  texture1d.cpp
  texture2d.cpp
  texture3d.cpp
  ${YAVIN_SOURCES}
  ${ARGN})

target_compile_features(unittests PRIVATE cxx_std_17)
target_link_libraries(unittests
  OpenGL::OpenGL
  OpenGL::GLU
  OpenGL::EGL
  OpenGL::GLX
  ${GLEW_LIBRARY}
  ${X11_LIBRARIES}
  Catch2::Catch2)
target_include_directories(unittests
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/single_include
    ${CMAKE_BINARY_DIR}/include
    ${GLEW_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIRS}
    ${X11_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR})
target_compile_options(unittests
  PRIVATE
    -Wall
    -Wextra
    -Wno-missing-braces
    -pedantic
)
if(PNG_FOUND AND PNG++_FOUND)
  target_include_directories(unittests PUBLIC
                             ${PNG_INCLUDE_DIRS} ${PNG++_INCLUDE_DIRS})
  target_link_libraries(unittests
                        ${PNG_LIBRARIES} ${PNG++_LIBRARIES})
endif()
catch_discover_tests(unittests)

################################################################################
# COVERAGE SECTION 
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  include(CodeCoverage)
  option(COVERAGE "compile with coverage flags" OFF)
  if (COVERAGE AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(FATAL_ERROR
      "code coverage can only be done with g++ compiler at the moment")
  endif()
endif()

if (COVERAGE)
  target_compile_options(unittests
    PUBLIC -fprofile-arcs -ftest-coverage)

  target_link_libraries(unittests -lgcov)

  set(EXCLUDES
    "${CMAKE_SOURCE_DIR}/test"
    "${CMAKE_SOURCE_DIR}/external"
    "/usr/include")

  set(COVERAGE_GCOVR_EXCLUDES ${EXCLUDES})
  set(COVERAGE_LCOV_EXCLUDES ${EXCLUDES})

  SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(
    NAME coverage.html
    EXECUTABLE unittests DEPENDENCIES unittests)

  SETUP_TARGET_FOR_COVERAGE_GCOVR_XML(
    NAME coverage.xml
    EXECUTABLE unittests DEPENDENCIES unittests)

  add_custom_target(coverage 
  COMMAND coverage.html coverage.xml
  DEPENDS unittests
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

