#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <yavin/glx_context.h>
#include <yavin/egl_context.h>

int main(int argc, char** argv) {
  yavin::egl_context ctx{4, 5};
  return Catch::Session().run(argc, argv);
}
