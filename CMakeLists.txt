cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(yavin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for YCM
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-fcolor-diagnostics")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always")
endif()

# try to find required packages
find_path(GLEW_INCLUDE_DIR GL/glew.h)
find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s PATH_SUFFIXES lib64)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL)
find_package(PNG REQUIRED)
find_package(PNG++ REQUIRED)

# configuring catch2
set(CATCH2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/catch2/single_include)
add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE ${CATCH2_INCLUDE_DIR})

file(GLOB YAVIN_SOURCES yavin_include/*.cpp)
file(GLOB YAVIN_HEADERS yavin_include/*.h)

option(VERBOSE "verbose OpenGL calls" OFF)
if (VERBOSE)
  set(VERBOSE_GL "true")
else(VERBOSE)
  set(VERBOSE_GL "false")
endif(VERBOSE)
configure_file(yavin_include/gl_verbose.cpp.in yavin_include/gl_verbose.cpp)

# create library target
add_library(yavin SHARED ${YAVIN_SOURCES} ${CMAKE_BINARY_DIR}/yavin_include/gl_verbose.cpp ${YAVIN_HEADERS})
target_compile_definitions(yavin PRIVATE DLL_EXPORT)
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(yavin PRIVATE 
  ${CMAKE_BINARY_DIR}/yavin_include
)
target_include_directories(yavin PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${OPENGL_INCLUDE_DIR} 
  ${GLM_INCLUDE_DIRS} 
  ${GLFW3_INCLUDE_DIRECTORIES}
  ${GLEW_INCLUDE_DIR}
  )

target_link_libraries(yavin 
  ${OPENGL_gl_LIBRARY} 
  ${OPENGL_glu_LIBRARY}
  ${GLFW_LIBRARIES} 
  ${PNG_LIBRARIES} 
  ${PNG+_LIBRARIES}
  ${GLEW_LIBRARY}
  )

option(BUILD_TESTS "Build tests" OFF)
# This makes the project importable from the build directory
if (BUILD_TESTS)
  export(TARGETS yavin FILE yavinConfig.cmake)
  configure_file(yavin_include/shader_include_paths.h.in yavin_include/shader_include_paths.h)
  add_subdirectory(test)
endif (BUILD_TESTS)

 # first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Generate documentation" OFF)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND AND BUILD_DOC)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    configure_file(external/doxygen-bootstrapped/example-site/header.html doc/header.html COPYONLY)
    configure_file(external/doxygen-bootstrapped/example-site/footer.html doc/footer.html COPYONLY)
    configure_file(external/doxygen-bootstrapped/customdoxygen.css doc/html/customdoxygen.css COPYONLY)
    configure_file(external/doxygen-bootstrapped/doxy-boot.js doc/html/doxy-boot.js COPYONLY)
    configure_file(external/doxygen-bootstrapped/jquery.smartmenus.js doc/html/jquery.smartmenus.js COPYONLY)
    configure_file(external/doxygen-bootstrapped/jquery.smartmenus.min.js doc/html/jquery.smartmenus.min.js COPYONLY)
    configure_file(external/doxygen-bootstrapped/addons/bootstrap/jquery.smartmenus.bootstrap.css doc/html/jquery.smartmenus.bootstrap.css COPYONLY)
    configure_file(external/doxygen-bootstrapped/addons/bootstrap/jquery.smartmenus.bootstrap.js doc/html/jquery.smartmenus.bootstrap.js COPYONLY)
    configure_file(external/doxygen-bootstrapped/addons/bootstrap/jquery.smartmenus.bootstrap.min.js doc/html/jquery.smartmenus.bootstrap.min.js COPYONLY)
    file(COPY ${CMAKE_SOURCE_DIR}/external/doxygen-bootstrapped/addons DESTINATION ${CMAKE_BINARY_DIR}/doc)

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    else (DOXYGEN_FOUND AND BUILD_DOC)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND AND BUILD_DOC)
