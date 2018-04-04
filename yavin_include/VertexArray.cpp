#include "VertexArray.h"
#include "gl_includes.h"

namespace Yavin {
VertexArray::VertexArray() {
  glCreateVertexArrays(1, &m_id);
  gl_error_check("glGenVertexArrays");
}
VertexArray::VertexArray(const VertexArray& other) {
  glCreateVertexArrays(1, &m_id);
  gl_error_check("glGenVertexArrays");
}
VertexArray::VertexArray(VertexArray&& other) {
  other.m_delete = false;
  m_id           = other.m_id;
}

VertexArray::~VertexArray() {
  if (m_delete) {
    glDeleteVertexArrays(1, &m_id);
    gl_error_check("glDeleteVertexArrays");
  }
}

void VertexArray::bind() {
  glBindVertexArray(m_id);
  gl_error_check("glBindVertexArray");
}

void VertexArray::unbind() {
  glBindVertexArray(0);
  gl_error_check("glBindVertexArray");
}

void VertexArray::draw(Primitive primitive, size_t num_primitives) {
  glDrawElements(primitive, num_primitives, UINT, 0);
  gl_error_check("glDrawElements");
}

void VertexArray::draw_points(size_t num_primitives) {
  draw(POINTS, num_primitives);
}
void VertexArray::draw_line_strip(size_t num_primitives) {
  draw(LINE_STRIP, num_primitives);
}
void VertexArray::draw_line_loop(size_t num_primitives) {
  draw(LINE_LOOP, num_primitives);
}
void VertexArray::draw_lines(size_t num_primitives) {
  draw(LINES, num_primitives);
}
void VertexArray::draw_line_strip_adjacency(size_t num_primitives) {
  draw(LINE_STRIP_ADJACENCY, num_primitives);
}
void VertexArray::draw_triangle_strip(size_t num_primitives) {
  draw(TRIANGLE_STRIP, num_primitives);
}
void VertexArray::draw_triangle_fan(size_t num_primitives) {
  draw(TRIANGLE_FAN, num_primitives);
}
void VertexArray::draw_triangles(size_t num_primitives) {
  draw(TRIANGLES, num_primitives);
}
void VertexArray::draw_triangle_strip_adjacency(size_t num_primitives) {
  draw(TRIANGLE_STRIP_ADJACENCY, num_primitives);
}
void VertexArray::draw_triangles_adjacency(size_t num_primitives) {
  draw(TRIANGLES_ADJACENCY, num_primitives);
}
void VertexArray::draw_patches(size_t num_primitives) {
  draw(PATCHES, num_primitives);
}
}  // namespace Yavin
