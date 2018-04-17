#include <catch.hpp>
#include <yavin>

using namespace Yavin;

TEST_CASE("initializer_list") {
  tuple<vec3, vec2, vec3> t{vec3{1, 2, 3}, vec2{4, 5}, vec3{6, 7, 8}};
  auto                    buffer = reinterpret_cast<float*>(&t);

  REQUIRE(buffer[0] == 1);
  REQUIRE(buffer[1] == 2);
  REQUIRE(buffer[2] == 3);
  REQUIRE(buffer[3] == 4);
  REQUIRE(buffer[4] == 5);
  REQUIRE(buffer[5] == 6);
  REQUIRE(buffer[6] == 7);
  REQUIRE(buffer[7] == 8);
}

TEST_CASE("make_tuple") {
  auto t      = make_tuple(vec3{1, 2, 3}, vec2{4, 5}, vec3{6, 7, 8});
  auto buffer = reinterpret_cast<float*>(&t);

  REQUIRE(buffer[0] == 1);
  REQUIRE(buffer[1] == 2);
  REQUIRE(buffer[2] == 3);
  REQUIRE(buffer[3] == 4);
  REQUIRE(buffer[4] == 5);
  REQUIRE(buffer[5] == 6);
  REQUIRE(buffer[6] == 7);
  REQUIRE(buffer[7] == 8);
}
