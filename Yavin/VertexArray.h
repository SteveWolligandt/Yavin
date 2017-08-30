#ifndef __YAVIN_VERTEXARRAY_H__
#define __YAVIN_VERTEXARRAY_H__

#include <iostream>
#include "IndexBuffer.h"
#include "Primitive.h"
#include "Type.h"
#include "VertexBuffer.h"
#include "error_check.h"
#include "gl_includes.h"

namespace Yavin {
class VertexArray {
 public:
  VertexArray();
  VertexArray(const VertexArray& other);
  VertexArray(VertexArray&& other);
  ~VertexArray();

  void bind();
  void unbind();
  void draw(Primitive primitive, unsigned int numPrimitives);

 protected:
  GLuint m_vaoID;
};
}  // namespace Yavin

#endif