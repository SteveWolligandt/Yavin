#ifndef __YAVIN_VERTEXARRAY_H__
#define __YAVIN_VERTEXARRAY_H__

#include <iostream>
#include "IndexBuffer.h"
#include "Primitive.h"
#include "Type.h"
#include "VertexBuffer.h"
#include "dll_export.h"
#include "error_check.h"

namespace Yavin {
class VertexArray {
 public:
  DLL_API VertexArray();
  DLL_API VertexArray(const VertexArray& other);
  DLL_API VertexArray(VertexArray&& other);
  DLL_API ~VertexArray();

  DLL_API void bind();
  DLL_API void unbind();
  DLL_API void draw(Primitive primitive, size_t num_primitives);
  DLL_API void draw(size_t num_primitives);
  DLL_API void draw_points(size_t num_primitives);
  DLL_API void draw_line_strip(size_t num_primitives);
  DLL_API void draw_line_loop(size_t num_primitives);
  DLL_API void draw_lines(size_t num_primitives);
  DLL_API void draw_line_strip_adjacency(size_t num_primitives);
  DLL_API void draw_triangle_strip(size_t num_primitives);
  DLL_API void draw_triangle_fan(size_t num_primitives);
  DLL_API void draw_triangles(size_t num_primitives);
  DLL_API void draw_triangle_strip_adjacency(size_t num_primitives);
  DLL_API void draw_triangles_adjacency(size_t num_primitives);
  DLL_API void draw_patches(size_t num_primitives);

 protected:
  unsigned int m_id;
  bool         m_delete = true;
};
}  // namespace Yavin

#endif
