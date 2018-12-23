include(CodeCoverage)

configure_file(rgb.png rgb.png COPYONLY)
configure_file(rgb2.png rgb2.png COPYONLY)
configure_file(bunny.obj bunny.obj COPYONLY)
configure_file(quad.obj quad.obj COPYONLY)
configure_file(oit_test.frag oit_test.frag COPYONLY)
configure_file(oit_test.vert oit_test.vert COPYONLY)

set(MAIN main.cpp)

function(test name)
  add_executable (${name} ${MAIN} ${ARGN})
  target_link_libraries(${name} yavin Catch2)
endfunction(test)

set(BUFFER_TEST_FILES buffer_test.cpp)
set(TUPLE_TEST_FILES tuple.cpp)
set(OIT_TEST_FILES oit_test.cpp)

test(buffer.test ${BUFFER_TEST_FILES})
test(tuple.test  ${TUPLE_TEST_FILES})
# test(oit.test    ${OIT_TEST_FILES})
test(all.test    ${MAIN} ${BUFFER_TEST_FILES} ${TUPLE_TEST_FILES} )

APPEND_COVERAGE_COMPILER_FLAGS()
set(COVERAGE_GCOVR_EXCLUDES  "${CMAKE_SOURCE_DIR}/test" "${CMAKE_SOURCE_DIR}/external")
SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(NAME coverage EXECUTABLE all.test DEPENDENCIES yavin Catch2)
