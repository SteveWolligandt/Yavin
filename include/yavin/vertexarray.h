#ifndef __YAVIN_VERTEXARRAY_H__
#define __YAVIN_VERTEXARRAY_H__

#include <iostream>
#include "indexbuffer.h"
#include "primitive.h"
#include "type.h"
#include "vertexbuffer.h"
#include "dllexport.h"
#include "glfunctions.h"

//==============================================================================
namespace yavin {
//==============================================================================
class vertexarray {
 public:
  using this_t = vertexarray;

  DLL_API vertexarray();
  DLL_API vertexarray(const vertexarray& other) = delete;
  DLL_API vertexarray(vertexarray&& other);
  DLL_API vertexarray& operator=(const vertexarray& other) = delete;
  DLL_API vertexarray& operator=(vertexarray&& other);
  DLL_API ~vertexarray();

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
}  // namespace yavin
//==============================================================================

#endif
