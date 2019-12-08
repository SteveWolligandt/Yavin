#include <yavin>
#include <catch2/catch.hpp>
//==============================================================================
namespace yavin::test {
//==============================================================================
TEST_CASE("framebuffer0", "[framebuffer]") {
  vertexbuffer<vec2> vbo{{0,0}, {1,0}, {1,1}, {0,1}};
  indexbuffer ibo{0,1,2, /*2,3,0*/};
  vertexarray vao;
  vao.bind();
  vbo.bind();
  ibo.bind();
  vbo.activate_attributes();

  orthographiccamera cam{0,1,0,1,-1,1, 1024,1024};
  shader s;
  s.add_stage<vertexshader>(
      "#version 450\n"
      "uniform mat4 proj;\n"
      "layout(location = 0) in vec2 pos;\n"
      "void main() {\n"
      "  gl_Position = proj * vec4(pos, 0, 1);\n"
      "}",
      shaderstageparser::SOURCE);

  s.add_stage<fragmentshader>(
      "#version 450\n"
      "layout(location = 0) out vec3 frag_color;\n"
      "void main() {\n"
      "  frag_color = vec3(0,0,1);\n"
      "}",
      shaderstageparser::SOURCE);
  s.create();
  s.bind();
  s.set_uniform("proj", cam.projection_matrix());

  tex2rgb<float> col{1024, 1024}; 
  texdepth depth{1024, 1024};
  framebuffer fbo{col, depth};
  fbo.bind();
  gl::clear_color(255, 0, 0, 0);
  clear_color_buffer();

  gl::viewport(cam.viewport());
  vao.draw_triangles(3);
  fbo.unbind();
  col.write_png("fbo_test.png");
}
//==============================================================================
}  // namespace yavin::test
//==============================================================================
