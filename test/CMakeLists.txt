configure_file(rgb.png rgb.png COPYONLY)
configure_file(rgb2.png rgb2.png COPYONLY)
configure_file(bunny.obj bunny.obj COPYONLY)
configure_file(quad.obj quad.obj COPYONLY)
configure_file(oit_test.frag oit_test.frag COPYONLY)
configure_file(oit_test.vert oit_test.vert COPYONLY)

set(MAIN main.cpp)

include(CodeCoverage)
APPEND_COVERAGE_COMPILER_FLAGS()
function(test name)
  add_executable (${name} 
    ${MAIN} 
    ${CMAKE_BINARY_DIR}/yavin_include/gl_verbose.cpp
    ${YAVIN_SOURCES}
    ${ARGN})
  target_link_libraries(${name}
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARIES}
    # ${PNG_LIBRARIES}
    # ${PNG+_LIBRARIES}
    ${GLEW_LIBRARY}
   Catch2)
  target_include_directories(${name} PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${OPENGL_INCLUDE_DIR} ${GLM_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRECTORIES} ${GLEW_INCLUDE_DIR})
  target_include_directories(${name} PRIVATE ${CMAKE_BINARY_DIR}/yavin_include)
endfunction(test)

set(INDEXBUFFER_TEST_FILES indexbuffer.cpp)
set(VERTEXBUFFER_TEST_FILES vertexbuffer.cpp)
set(TUPLE_TEST_FILES tuple.cpp)
set(OIT_TEST_FILES oit_test.cpp)
set(ALL_TEST_FILES ${MAIN} 
  ${INDEXBUFFER_TEST_FILES}
  ${VERTEXBUFFER_TEST_FILES}
  ${TUPLE_TEST_FILES}
)
test(indexbuffer.test ${INDEXBUFFER_TEST_FILES})
test(vertexbuffer.test ${VERTEXBUFFER_TEST_FILES})
test(tuple.test ${TUPLE_TEST_FILES})
test(oit.test ${OIT_TEST_FILES})

test(all.test ${ALL_TEST_FILES})
set(EXCLUDES "${CMAKE_SOURCE_DIR}/test" "${CMAKE_SOURCE_DIR}/external" "/usr/include")
set(COVERAGE_GCOVR_EXCLUDES ${EXCLUDES})
set(COVERAGE_LCOV_EXCLUDES ${EXCLUDES})
SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(NAME coverage.html EXECUTABLE all.test DEPENDENCIES all.test)
SETUP_TARGET_FOR_COVERAGE_GCOVR_XML(NAME coverage.xml EXECUTABLE all.test DEPENDENCIES all.test)

add_custom_target(coverage 
  COMMAND coverage.html coverage.xml
  DEPENDS all.test
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
