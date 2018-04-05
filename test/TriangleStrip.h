#ifndef __YAVIN_TEST_TRIANGLESTRIP_H__
#define __YAVIN_TEST_TRIANGLESTRIP_H__
#include <yavin>
//==============================================================================
namespace Yavin::Test {
//==============================================================================
class TriangleStrip {
 public:
  using vbo_t = VertexBuffer<vec3>;
  TriangleStrip(std::initializer_list<vbo_t::data_t>&& vertices)
      : size(vertices.size()), vbo(std::move(vertices)) {
    std::vector<unsigned int> indices;
    for (size_t i = 0; i < size; ++i) ibo.push_back(i);
    ibo.upload_data();

    vao.bind();
    vbo.bind();
    ibo.bind();
    vbo_t::activate_attributes();
    vao.unbind();
  }

  TriangleStrip(const TriangleStrip& other)
      : size(other.size), vao(other.vao), vbo(other.vbo), ibo(other.ibo) {
    vao.bind();
    vbo.bind();
    ibo.bind();
    vbo_t::activate_attributes();
    vao.unbind();
  }

  void draw() {
    vao.bind();
    vao.draw(Yavin::TRIANGLE_STRIP, size);
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