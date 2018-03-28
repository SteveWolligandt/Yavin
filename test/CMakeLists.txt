configure_file(shaders/passthrough.vert shaders/passthrough.vert COPYONLY)
configure_file(shaders/passthrough.frag shaders/passthrough.frag COPYONLY)
configure_file(rgb.png rgb.png COPYONLY)
configure_file(rgb2.png rgb2.png COPYONLY)
configure_file(bunny.obj bunny.obj COPYONLY)
configure_file(quad.obj quad.obj COPYONLY)

add_executable (vbo_test vbo_test.cpp)
target_link_libraries(vbo_test yavin)
