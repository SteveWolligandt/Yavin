#include <yavin/atomiccounterbuffer.h>
#include <catch2/catch.hpp>
//==============================================================================
namespace yavin::test {
//==============================================================================
TEST_CASE("atomiccounterbuffer_0",
          "[abo][atomiccounterbuffer][initializer_list][map][read]") {
  atomiccounterbuffer abo{1, 2, 3};
  auto abomap = abo.map();
  REQUIRE(abo.size() == 3);
  REQUIRE(abomap[0] == 1);
  REQUIRE(abomap[1] == 2);
  REQUIRE(abomap[2] == 3);
}
//==============================================================================
TEST_CASE("atomiccounterbuffer_1",
          "[abo][atomiccounterbuffer][initializer_list][index][read]") {
  atomiccounterbuffer abo{1, 2, 3};
  REQUIRE(abo.size() == 3);
  REQUIRE(abo[0] == 1);
  REQUIRE(abo[1] == 2);
  REQUIRE(abo[2] == 3);
}
//==============================================================================
TEST_CASE("atomiccounterbuffer_2",
          "[abo][atomiccounterbuffer][initializer_list][index][write]") {
  atomiccounterbuffer abo{0,0,0};
  REQUIRE(abo.size() == 3);
  abo[1] = 3;
  REQUIRE(abo[0] == 0);
  REQUIRE(abo[1] == 3);
  REQUIRE(abo[2] == 0);
}
//==============================================================================
TEST_CASE("atomiccounterbuffer_3",
          "[abo][atomiccounterbuffer][initializer_list][map][write]") {
  atomiccounterbuffer abo(3);
  REQUIRE(abo.size() == 3);
  auto abomap = abo.map();
  abomap[0] = 1;
  abomap[1] = 3;
  abomap[2] = 2;
  abomap.unmap();

  REQUIRE(abo[0] == 1);
  REQUIRE(abo[1] == 3);
  REQUIRE(abo[2] == 2);
}
//==============================================================================
}  // namespace yavin::test
//==============================================================================
