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
  std::cout << "COPY CONSTRUCTOR OF VAO\n";
}
VertexArray::VertexArray(VertexArray&& other) {
  std::cout << "MOVE CONSTRUCTOR OF VAO\n";
}

VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &m_id);
  gl_error_check("glDeleteVertexArrays");
}

void VertexArray::bind() {
  glBindVertexArray(m_id);
  gl_error_check("glBindVertexArray");
}

void VertexArray::unbind() {
  glBindVertexArray(0);
  gl_error_check("glBindVertexArray");
}

void VertexArray::draw(Primitive primitive, unsigned int numPrimitives) {
  glDrawElements(primitive, numPrimitives, UINT, 0);
  gl_error_check("glDrawElements");
}
}  // namespace Yavin
