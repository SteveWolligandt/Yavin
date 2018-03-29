#ifndef __YAVIN_VERTEXARRAY_H__
#define __YAVIN_VERTEXARRAY_H__

#include <iostream>
#include "IndexBuffer.h"
#include "Primitive.h"
#include "Type.h"
#include "VertexBuffer.h"
#include "error_check.h"
#include "dll_export.h"

namespace Yavin {
class VertexArray {
 public:
  DLL_API VertexArray();
  DLL_API VertexArray(const VertexArray& other);
  DLL_API VertexArray(VertexArray&& other);
  DLL_API ~VertexArray();

  DLL_API void bind();
  DLL_API void unbind();
  DLL_API void draw(Primitive primitive, unsigned int num_primitives);

 protected:
  unsigned int m_id;
};
}  // namespace Yavin

#endif