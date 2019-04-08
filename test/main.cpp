#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <yavin/context.h>

int main( int argc, char* argv[] ) {
 auto context = std::make_unique<yavin::Context>(4,5);
 return Catch::Session().run(argc, argv);
}
