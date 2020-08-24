enable_testing()
include(CTest)
include(Catch)
#-------------------------------------------------------------------------------
add_executable (window.test ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp)
target_link_libraries(window.test yavin Catch2::Catch2)
target_compile_features(window.test PUBLIC cxx_std_20)
target_compile_options(window.test PUBLIC
  -Wall -Wextra -pedantic -Wno-missing-braces -Wno-unused-lambda-capture)
add_custom_target (window.test.run COMMAND ./window.test DEPENDS window.test)
#-------------------------------------------------------------------------------
set(YAVIN_TEST_CPPS main.cpp)
macro(create_test NAME)
  list(APPEND YAVIN_TEST_CPPS "${NAME}.cpp")
  add_executable (${NAME}.test ${NAME}.cpp main.cpp)
  target_link_libraries(${NAME}.test yavin Catch2::Catch2)
  target_compile_features(${NAME}.test PUBLIC cxx_std_20)
  target_compile_options(${NAME}.test PUBLIC
    -Wall -Wextra -pedantic -Wno-missing-braces -Wno-unused-lambda-capture)
  add_custom_target (${NAME}.test.run COMMAND ./${NAME}.test --use-colour=yes DEPENDS ${NAME}.test)
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
     "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      add_custom_target (${NAME}.test.valgrind
                         COMMAND valgrind --log-file="${NAME}.test.valgrind" --tool=memcheck --leak-check=yes --show-reachable=yes --track-fds=yes ./${NAME}.test
                         DEPENDS ${NAME}.test)
    endif()
  endif()
  add_custom_target (${NAME}.test.callgrind
                     COMMAND valgrind --tool=callgrind --callgrind-out-file=${NAME}.test.callgrind ./${NAME}.test
                     DEPENDS ${NAME}.test)
  add_custom_target (${NAME}.test.callgrind.kcachegrind
                     COMMAND kcachegrind ${NAME}.test.callgrind
                     DEPENDS ${NAME}.test.callgrind)
  add_custom_target (${NAME}.test.callgrind.dot
                     COMMAND gprof2dot --format=callgrind --output=${NAME}.test.dot ${NAME}.test.callgrind
                     COMMAND dot -Tsvg ${NAME}.test.dot -o ${NAME}.test-graph.svg
                     DEPENDS ${NAME}.test.callgrind)
endmacro()
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# create tests from cpp files in current directory 
file(GLOB YAVIN_TEST_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
list(REMOVE_ITEM YAVIN_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
list(REMOVE_ITEM YAVIN_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/window.cpp")
foreach(FILE ${YAVIN_TEST_FILES})
  get_filename_component(FILENAME ${FILE} NAME_WE)
  create_test(${FILENAME})
endforeach()

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# create one executable with all tests
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
set(YAVIN_UNITTEST_SOURCES
      main.cpp
      ${YAVIN_TEST_FILES}
      ${YAVIN_SOURCES}
      ${YAVIN_IMGUI_FILES}
      ${ARGN})

set(YAVIN_UNITTEST_INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/single_include
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/single_include
    ${OPENGL_INCLUDE_DIR} 
    ${YAVIN_GLEW_INCLUDE_DIR}
    ${YAVIN_IMGUI_INCLUDE_DIRS}
    ${X11_INCLUDE_DIR})
set(YAVIN_UNITTEST_LIBRARIES
  OpenGL::OpenGL
  OpenGL::GLU
  ${YAVIN_GLEW_LIBRARY}
  Catch2::Catch2
  ${X11_LIBRARIES}
  OpenGL::EGL)

if(YAVIN_USE_EGL)
  list(APPEND YAVIN_UNITTEST_LIBRARIES OpenGL::EGL)
elseif(YAVIN_USE_GLX)
  list(APPEND YAVIN_UNITTEST_LIBRARIES OpenGL::GLX ${X11_LIBRARIES})
  list(APPEND YAVIN_UNITTEST_INCLUDE_DIRECTORIES ${X11_INCLUDE_DIR})
endif()

if(PNG_FOUND AND PNG++_FOUND)
  list(APPEND YAVIN_UNITTEST_INCLUDE_DIRECTORIES
    ${PNG_INCLUDE_DIRS} ${PNG++_INCLUDE_DIRS})
  list(APPEND YAVIN_UNITTEST_LIBRARIES
    ${PNG_LIBRARIES} ${PNG++_LIBRARIES})
endif()
add_executable(unittests ${YAVIN_UNITTEST_SOURCES})
target_include_directories(unittests PUBLIC ${YAVIN_UNITTEST_INCLUDE_DIRECTORIES})
target_link_libraries(unittests PUBLIC ${YAVIN_UNITTEST_LIBRARIES})
target_compile_features(unittests PUBLIC cxx_std_20)
target_compile_options(unittests PUBLIC -Wall -Wextra -Wno-missing-braces -pedantic)
catch_discover_tests(unittests)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# custom targets for unittests
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
add_custom_target (unittests.run COMMAND ./unittests --use-colour=yes DEPENDS unittests)
add_custom_target (unittests.xml COMMAND ./unittests --reporter=xml DEPENDS unittests)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
   "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_custom_target (unittests.valgrind
                     COMMAND valgrind --log-file="unittests.valgrind" --tool=memcheck --leak-check=yes --show-reachable=yes --track-fds=yes ./unittests
                     DEPENDS unittests)
  else()
  add_custom_target (unittests.valgrind
                     COMMAND echo "" > /dev/null
                     COMMENT "Target ${BoldRed}unittest.valgrind ${ColourReset}only available in debug mode.")
  endif()
endif()
add_custom_target (
  unittests.callgrind
  COMMAND
    valgrind --tool=callgrind --callgrind-out-file=unittests.callgrind ./unittests
  DEPENDS
    unittests)
add_custom_target (
  unittests.callgrind.kcachegrind
  COMMAND
    kcachegrind unittests.callgrind
  DEPENDS
    unittests.callgrind)
add_custom_target (
  unittests.callgrind.dot
  COMMAND
    gprof2dot --format=callgrind --output=unittests.dot unittests.callgrind
  COMMAND
    dot -Tsvg unittests.dot -o unittests-graph.svg
  DEPENDS
    unittests.callgrind)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# COVERAGE
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND (
  "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
  "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
  include(CodeCoverage)
  add_executable (coverage.gcov ${YAVIN_UNITTEST_SOURCES})
  target_compile_features(coverage.gcov PUBLIC cxx_std_20)
  target_include_directories(coverage.gcov PUBLIC ${YAVIN_UNITTEST_INCLUDE_DIRECTORIES})
  set(COVERAGE_EXCLUDES
      "${CMAKE_SOURCE_DIR}/test"
      "${CMAKE_SOURCE_DIR}/external"
      "/usr/include")
  set(COVERAGE_GCOVR_EXCLUDES ${COVERAGE_EXCLUDES})

  SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(
    NAME coverage.gcov.html
    EXECUTABLE coverage.gcov DEPENDENCIES coverage.gcov)

  SETUP_TARGET_FOR_COVERAGE_GCOVR_XML(
    NAME coverage.gcov.xml
    EXECUTABLE coverage.gcov DEPENDENCIES coverage.gcov)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(coverage.gcov ${YAVIN_UNITTEST_LIBRARIES} gcov)
    target_compile_options(coverage.gcov
      PUBLIC
        -Wall -Wextra -pedantic
        -fprofile-arcs -ftest-coverage -fno-inline
        -fno-inline-small-functions -fno-default-inline
        -Wno-missing-braces -Wno-unused-lambda-capture)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      target_link_libraries(coverage.gcov
        ${YAVIN_UNITTEST_LIBRARIES} gcov -fprofile-instr-generate -fcoverage-mapping)
      target_compile_options(coverage.gcov
        PUBLIC
          -Wall -Wextra -pedantic
          -fprofile-arcs -ftest-coverage -fno-inline
          -Wno-missing-braces -Wno-unused-lambda-capture)
    # LLVM Sanitizer
    add_executable (coverage.llvm ${YAVIN_UNITTEST_SOURCES})
    target_compile_features(coverage.llvm PUBLIC cxx_std_20)
    target_include_directories(coverage.llvm PUBLIC ${YAVIN_UNITTEST_INCLUDE_DIRECTORIES})
    target_link_libraries(coverage.llvm
      ${YAVIN_UNITTEST_LIBRARIES} -fprofile-instr-generate -fcoverage-mapping)
    target_compile_options(coverage.llvm
      PUBLIC
        -Wall -Wextra -pedantic
        -fprofile-instr-generate -fcoverage-mapping
        -Wno-missing-braces -Wno-unused-lambda-capture)

    set(YAVIN_TEST_COVBIN_PROFRAW
        ${CMAKE_CURRENT_BINARY_DIR}/coverage.llvm.profraw)
    set(TATTOINE_TEST_COVBIN_PROFDATA
        ${CMAKE_CURRENT_BINARY_DIR}/coverage.llvm.profdata)
    add_custom_target(coverage.llvm.profraw 
      COMMAND LLVM_PROFILE_FILE=${YAVIN_TEST_COVBIN_PROFRAW} ./coverage.llvm
      DEPENDS coverage.llvm
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_custom_target(
      coverage.llvm.profdata 
      COMMAND
        llvm-profdata merge -sparse ${YAVIN_TEST_COVBIN_PROFRAW} -o ${TATTOINE_TEST_COVBIN_PROFDATA}
      DEPENDS coverage.llvm.profraw
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_custom_target(coverage.llvm.line 
      COMMAND llvm-cov show ./coverage.llvm -instr-profile=${TATTOINE_TEST_COVBIN_PROFDATA} > coverage.txt
      DEPENDS coverage.llvm.profdata
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_custom_target(coverage.llvm.html 
      COMMAND llvm-cov show ./coverage.llvm -format=html -instr-profile=${TATTOINE_TEST_COVBIN_PROFDATA} > coverage.html
      DEPENDS coverage.llvm.profdata
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  endif()
endif()
