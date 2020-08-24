#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <yavin/context.h>

int main(int argc, char** argv) {
  yavin::context ctx;
  return Catch::Session().run(argc, argv);
}
