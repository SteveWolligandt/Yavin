#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <yavin>

int main(int argc, char** argv) {
  using namespace yavin::glx;
  window ctx{"glx test window", 4, 5};
  return Catch::Session().run(argc, argv);
}

//==============================================================================
namespace yavin::test {
//==============================================================================
TEST_CASE(
    "egl_window"
    "[egl_window]") {
  while (true) {}
}
//==============================================================================
}  // namespace yavin::test
//==============================================================================
