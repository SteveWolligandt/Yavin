#ifndef __YAVIN_VERTEXARRAY_H__
#define __YAVIN_VERTEXARRAY_H__

#include <iostream>
#include "IndexBuffer.h"
#include "Primitive.h"
#include "Type.h"
#include "VertexBuffer.h"
#include "error_check.h"

namespace Yavin {
class VertexArray {
 public:
  VertexArray();
  VertexArray(const VertexArray& other);
  VertexArray(VertexArray&& other);
  ~VertexArray();

  void bind();
  void unbind();
  void draw(Primitive primitive, unsigned int num_primitives);

 protected:
  unsigned int m_id;
};
}  // namespace Yavin

#endif