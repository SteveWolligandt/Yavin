#include <catch2/catch.hpp>
#include <yavin>

//==============================================================================
namespace yavin::test {
//==============================================================================

extern Window window;

//==============================================================================
TEST_CASE("[VBO] download") {
  using vbo_t = VertexBuffer<vec2, vec2>;
  vbo_t vbo{{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};

  auto down = vbo.download_data();
  REQUIRE(get<0>(down[0])[0] == 1);
  REQUIRE(get<0>(down[0])[1] == 2);
  REQUIRE(get<1>(down[0])[0] == 3);
  REQUIRE(get<1>(down[0])[1] == 4);
  REQUIRE(get<0>(down[1])[0] == 5);
  REQUIRE(get<0>(down[1])[1] == 6);
  REQUIRE(get<1>(down[1])[0] == 7);
  REQUIRE(get<1>(down[1])[1] == 8);
}

//==============================================================================
TEST_CASE("[VBO] element access") {
  using vbo_t = VertexBuffer<vec3>;
  REQUIRE(vbo_t::data_size == 3 * sizeof(float));

  vbo_t vbo{{1, 2, 3}, {4, 5, 6}};
  REQUIRE(vbo.size() == 2);
  REQUIRE(vbo.capacity() == 2);

  vbo_t::data_t front   = vbo[0];
  vbo_t::data_t back    = vbo[1];
  auto          v_front = get<0>(front);
  auto          v_back  = get<0>(back);

  REQUIRE(v_front[0] == 1);
  REQUIRE(v_front[1] == 2);
  REQUIRE(v_front[2] == 3);
  REQUIRE(v_back[0] == 4);
  REQUIRE(v_back[1] == 5);
  REQUIRE(v_back[2] == 6);
}

//==============================================================================
}  // namespace yavin::test
//==============================================================================
