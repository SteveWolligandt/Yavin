#include <catch2/catch.hpp>
#include <yavin>
//==============================================================================
namespace yavin::test {
//==============================================================================
TEST_CASE("tuple_cast") {
  tuple t{std::array{1.0f, 2.0f}, std::array{3.0f, 4.0f}, 5.0f};
  auto                       buffer = t.as_pointer<float>();
  REQUIRE(buffer[0] == 1);
  REQUIRE(buffer[1] == 2);
  REQUIRE(buffer[2] == 3);
  REQUIRE(buffer[3] == 4);
  REQUIRE(buffer[4] == 5);
}
//==============================================================================
TEST_CASE("tuple_assign") {
  tuple t{std::array{1.0f, 2.0f}, 3.0f};
  t           = {std::array{3.0f, 4.0f}, 5.0f};
  auto buffer = t.as_pointer<float>();
  REQUIRE(buffer[0] == 3);
  REQUIRE(buffer[1] == 4);
  REQUIRE(buffer[2] == 5);
}
//==============================================================================
}  // namespace yavin::test
//==============================================================================
