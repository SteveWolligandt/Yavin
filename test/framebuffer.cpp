#include <yavin>
#include <catch2/catch.hpp>
//==============================================================================
namespace yavin::test {
//==============================================================================
TEST_CASE("framebuffer0", "[framebuffer]") {
  vertexbuffer<vec2> vbo{{-1,-1}, {1,-1}, {1,1}, {-1,1}};
  indexbuffer ibo{0,1,2, /*2,3,0*/};
  vertexarray vao;
  vao.bind();
  vbo.bind();
  ibo.bind();
  vbo.activate_attributes();

  tex2rgb<float> col; 
  texdepth depth;
  framebuffer fbo{col, depth};
  fbo.bind();
  gl::clear_color(0, 0, 0, 0);
  clear_color_depth_buffer();

  shader s;
  s.add_stage<vertexshader>(
      "#version 450\n"
      "layout(location = 0) in vec2 pos;\n"
      "void main() {\n"
      "  gl_Position = projection * vec4(pos, 1);\n"
      "}",
      shaderstageparser::SOURCE);
  s.add_stage<fragmentshader>(
      "#version 450"
      "layout(location = 0) out vec4 frag_color;\n"
      "void main() {"
      "  frag_color = vec3(1,1,1);"
      "}",
      shaderstageparser::SOURCE);
  s.create();
  s.bind();

  vao.draw_triangles(2);
  col.write_png("fbo_test.png");
}
//==============================================================================
}  // namespace yavin::test
//==============================================================================
