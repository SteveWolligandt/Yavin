#include "VertexArray.h"
#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

VertexArray::VertexArray() { gl::create_vertex_arrays(1, &m_id); }

VertexArray::VertexArray(const VertexArray& other) {
  glCreateVertexArrays(1, &m_id);
  gl_error_check("glGenVertexArrays");
}

VertexArray::VertexArray(VertexArray&& other) {
  other.m_delete = false;
  m_id           = other.m_id;
}

VertexArray::~VertexArray() {
  if (m_delete) gl::delete_vertex_arrays(1, &m_id);
}

void VertexArray::bind() const { gl::bind_vertex_array(m_id); }

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
}  // namespace Yavin
//==============================================================================
