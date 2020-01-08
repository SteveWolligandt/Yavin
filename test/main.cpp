#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <yavin/egl_context.h>
#include <yavin/glx_context.h>

int main(int argc, char** argv) {
  using namespace yavin::egl;
  context ctx{4, 5};
  return Catch::Session().run(argc, argv);
}
