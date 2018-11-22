#ifndef __YAVIN_VERTEXARRAY_H__
#define __YAVIN_VERTEXARRAY_H__

#include <iostream>
#include "IndexBuffer.h"
#include "Primitive.h"
#include "Type.h"
#include "VertexBuffer.h"
#include "dll_export.h"
#include "gl_functions.h"

//==============================================================================
namespace Yavin {
//==============================================================================
class VertexArray {
 public:
  using this_t = VertexArray;

  DLL_API VertexArray();
  DLL_API VertexArray(const VertexArray& other) = delete;
  DLL_API VertexArray(VertexArray&& other);
  DLL_API VertexArray& operator=(const VertexArray& other) = delete;
  DLL_API VertexArray& operator=(VertexArray&& other);
  DLL_API ~VertexArray();

  DLL_API void destroy_handle();
  DLL_API void bind() const;
  DLL_API void unbind() const;
  DLL_API void draw(Primitive primitive, size_t num_primitives) const;
  DLL_API void draw(size_t num_primitives) const;
  DLL_API void draw_points(size_t num_primitives) const;
  DLL_API void draw_line_strip(size_t num_primitives) const;
  DLL_API void draw_line_loop(size_t num_primitives) const;
  DLL_API void draw_lines(size_t num_primitives) const;
  DLL_API void draw_line_strip_adjacency(size_t num_primitives) const;
  DLL_API void draw_triangle_strip(size_t num_primitives) const;
  DLL_API void draw_triangle_fan(size_t num_primitives) const;
  DLL_API void draw_triangles(size_t num_primitives) const;
  DLL_API void draw_triangle_strip_adjacency(size_t num_primitives) const;
  DLL_API void draw_triangles_adjacency(size_t num_primitives) const;
  DLL_API void draw_patches(size_t num_primitives) const;

 protected:
  GLuint m_gl_handle;
};
//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
