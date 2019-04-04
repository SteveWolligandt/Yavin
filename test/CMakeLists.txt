find_package(Catch2 REQUIRED)
enable_testing()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  include(CodeCoverage)
  option(COVERAGE "compile with coverage flags" OFF)
  if (COVERAGE AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(FATAL_ERROR "code coverage can only be done with g++ compiler at the moment")
  endif()
endif()

include(CTest)
include(Catch)

configure_file(oit_test.frag oit_test.frag COPYONLY)
configure_file(oit_test.vert oit_test.vert COPYONLY)

set(MAIN main.cpp)

add_executable (unittests
  main.cpp
  # oit_test.cpp
  tuple.cpp
  indexbuffer.cpp
  vertexbuffer.cpp
  texture2d.cpp
  texture3d.cpp
  ${CMAKE_BINARY_DIR}/yavin_include/gl_verbose.cpp
  ${YAVIN_SOURCES}
  ${ARGN})
target_compile_features(unittests PUBLIC cxx_std_17)
target_link_libraries(unittests
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${GLEW_LIBRARY}
  Catch2::Catch2)
target_include_directories(unittests
  PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIRECTORIES}
    ${GLEW_INCLUDE_DIR})
target_include_directories(unittests
  PRIVATE
    ${CMAKE_BINARY_DIR}/yavin_include)
  target_compile_options(unittests
  PRIVATE
    -Wall -Wextra -pedantic
    -Wno-missing-braces -Wno-unused-lambda-capture)
catch_discover_tests(unittests)

if (COVERAGE)
  target_compile_options(unittests
  PRIVATE
    -fprofile-arcs -ftest-coverage)
endif()

if (COVERAGE)
  set(EXCLUDES "${CMAKE_SOURCE_DIR}/test" "${CMAKE_SOURCE_DIR}/external" "/usr/include")
  set(COVERAGE_GCOVR_EXCLUDES ${EXCLUDES})
  set(COVERAGE_LCOV_EXCLUDES ${EXCLUDES})
  SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(NAME coverage.html EXECUTABLE unittests DEPENDENCIES unittests)
  SETUP_TARGET_FOR_COVERAGE_GCOVR_XML(NAME coverage.xml EXECUTABLE unittests DEPENDENCIES unittests)
endif()

add_custom_target(coverage 
  COMMAND coverage.html coverage.xml
  DEPENDS unittests
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
