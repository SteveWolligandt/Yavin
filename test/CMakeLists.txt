cmake_minimum_required (VERSION 2.6)
project(yavin_testing)
set(CMAKE_CXX_STANDARD 17)

add_executable (window_test window_test.cpp)
configure_file(shaders/passthrough.vert shaders/passthrough.vert COPYONLY)
configure_file(shaders/passthrough.frag shaders/passthrough.frag COPYONLY)
configure_file(rgb.png rgb.png COPYONLY)
configure_file(rgb2.png rgb2.png COPYONLY)
configure_file(bunny.obj bunny.obj COPYONLY)
configure_file(quad.obj quad.obj COPYONLY)
target_link_libraries(window_test yavin)
include_directories(..)
add_custom_command(TARGET window_test PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/test/shaders $<TARGET_FILE_DIR:window_test>/shaders)

add_executable (basic_test test.cpp)
target_link_libraries(basic_test yavin)
include_directories(..)

add_executable (regex_glsl regex_glsl.cpp)
target_link_libraries(basic_test yavin)
include_directories(..)

add_executable (vbo_test vbo_test.cpp)
target_link_libraries(vbo_test yavin)
include_directories(..)
include_directories(${GL3W_INCLUDE_DIR})
