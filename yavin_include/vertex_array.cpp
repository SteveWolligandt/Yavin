#include "vertex_array.h"

//==============================================================================
namespace yavin {
//==============================================================================

VertexArray::VertexArray() { gl::create_vertex_arrays(1, &m_gl_handle); }

// VertexArray::VertexArray(const VertexArray& other) {
//   gl::create_vertex_arrays(1, &m_gl_handle);
// }

VertexArray::VertexArray(VertexArray&& other)
    : m_gl_handle(std::exchange(other.m_gl_handle, 0)) {}

// VertexArray& VertexArray::operator=(const VertexArray& other) {
//   destroy_handle();
//   gl::create_vertex_arrays(1, &m_gl_handle);
//   return *this;
// }

VertexArray& VertexArray::operator=(VertexArray&& other) {
  m_gl_handle = std::exchange(other.m_gl_handle, 0);
  return *this;
}

VertexArray::~VertexArray() { destroy_handle(); }

void VertexArray::destroy_handle() {
  if (m_gl_handle != 0) gl::delete_vertex_arrays(1, &m_gl_handle);
  m_gl_handle = 0;
}

void VertexArray::bind() const { gl::bind_vertex_array(m_gl_handle); }

void VertexArray::unbind() const { gl::bind_vertex_array(0); }

void VertexArray::draw(Primitive primitive, size_t num_primitives) const {
  gl::draw_elements(primitive, num_primitives, UINT, 0);
}

void VertexArray::draw_points(size_t num_primitives) const {
  draw(POINTS, num_primitives);
}

void VertexArray::draw_line_strip(size_t num_primitives) const {
  draw(LINE_STRIP, num_primitives);
}

void VertexArray::draw_line_loop(size_t num_primitives) const {
  draw(LINE_LOOP, num_primitives);
}

void VertexArray::draw_lines(size_t num_primitives) const {
  draw(LINES, num_primitives);
}

void VertexArray::draw_line_strip_adjacency(size_t num_primitives) const {
  draw(LINE_STRIP_ADJACENCY, num_primitives);
}

void VertexArray::draw_triangle_strip(size_t num_primitives) const {
  draw(TRIANGLE_STRIP, num_primitives);
}

void VertexArray::draw_triangle_fan(size_t num_primitives) const {
  draw(TRIANGLE_FAN, num_primitives);
}

void VertexArray::draw_triangles(size_t num_primitives) const {
  draw(TRIANGLES, num_primitives);
}

void VertexArray::draw_triangle_strip_adjacency(size_t num_primitives) const {
  draw(TRIANGLE_STRIP_ADJACENCY, num_primitives);
}

void VertexArray::draw_triangles_adjacency(size_t num_primitives) const {
  draw(TRIANGLES_ADJACENCY, num_primitives);
}

void VertexArray::draw_patches(size_t num_primitives) const {
  draw(PATCHES, num_primitives);
}

//==============================================================================
}  // namespace yavin
//==============================================================================
