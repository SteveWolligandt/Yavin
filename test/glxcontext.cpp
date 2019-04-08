#include <thread>
#include <yavin>

int main() {
  using namespace yavin;
  Context c{4, 5};

  tex2D<float, RGBA> tex{200, 200};
  framebuffer        fbo{tex};

  fbo.bind();
  gl::clear_color(0, 0.5, 1, 1);
  clear_color_buffer();
  tex.write_png("blue.png");

  gl::clear_color(1, 0.5, 0, 1);
  clear_color_buffer();
  tex.write_png("orange.png");
}
