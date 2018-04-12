configure_file(rgb.png rgb.png COPYONLY)
configure_file(rgb2.png rgb2.png COPYONLY)
configure_file(bunny.obj bunny.obj COPYONLY)
configure_file(quad.obj quad.obj COPYONLY)
configure_file(oit_test.frag oit_test.frag COPYONLY)
configure_file(oit_test.vert oit_test.vert COPYONLY)

add_executable(all_tests main.cpp vbo_test.cpp oit_test.cpp)

add_executable (vbo_test main.cpp vbo_test.cpp)
target_link_libraries(vbo_test yavin Catch2)

add_executable (oit_test main.cpp oit_test.cpp)
target_link_libraries(oit_test yavin Catch2)
