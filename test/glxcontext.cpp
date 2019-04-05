#include <yavin>
#include <thread>

int main() {
  using namespace yavin;
  Context c{4,5};

  FrameBuffer fbo;
  Texture2D<float, RGBA> tex{200,200};
  fbo.attachTex2D(tex);

  fbo.bind();
  gl::clear_color(0, 0.5, 1, 1);
  clear_color_buffer();
  tex.save_png("blue.png");

  gl::clear_color(1, 0.5, 0, 1);
  clear_color_buffer();
  tex.save_png("orange.png");
}
