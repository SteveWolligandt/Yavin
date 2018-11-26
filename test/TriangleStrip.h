#ifndef __YAVIN_TEST_TRIANGLESTRIP_H__
#define __YAVIN_TEST_TRIANGLESTRIP_H__

#include <vector>

//==============================================================================
namespace Yavin::Test {
//==============================================================================
class TriangleStrip {
 public:
  using vbo_t = VertexBuffer<vec3>;

  TriangleStrip(const std::initializer_list<vbo_t::data_t>& vertices)
      : size(vertices.size()), vbo(std::move(vertices)) {
    ibo.reserve(size);
    for (size_t i = 0; i < size; ++i) ibo.push_back(i);

    vao.bind();
    vbo.bind();
    ibo.bind();
    vbo.activate_attributes();
    vao.unbind();
  }

  TriangleStrip(const TriangleStrip& other)
      : size(other.size), vbo(other.vbo), ibo(other.ibo) {
    vao.bind();
    vbo.bind();
    ibo.bind();
    vao.unbind();
  }

  void draw() {
    vao.bind();
    vao.draw_triangle_strip(size);
  }

 private:
  size_t      size;
  VertexArray vao;
  vbo_t       vbo;
  IndexBuffer ibo;
};
//==============================================================================
}  // namespace Yavin::Test
//==============================================================================

#endif
