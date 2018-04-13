#include <catch.hpp>
#include <yavin>

using namespace Yavin;

Window w("Buffer Test", 100, 50);

TEST_CASE("IBO") {
  std::vector<unsigned int> correct_data{0, 1, 2};
  IndexBuffer               ibo{0, 1, 2};
  auto                      ibo_handle = ibo.gl_handle();
  IndexBuffer               copy_assigned{0, 1, 3, 4};
  copy_assigned         = ibo;
  auto copy_constructed = ibo;

  REQUIRE(ibo.size() == 3);
  REQUIRE(ibo.capacity() == 3);

  REQUIRE(copy_constructed.size() == 3);
  REQUIRE(copy_constructed.capacity() == 3);

  REQUIRE(copy_assigned.size() == 3);
  REQUIRE(copy_assigned.capacity() == 4);

  auto ibo_data              = ibo.download_data();
  auto copy_constructed_data = copy_constructed.download_data();
  auto copy_assigned_data    = copy_assigned.download_data();

  for (size_t i = 0; i < 3; ++i) {
    REQUIRE(correct_data[i] == ibo[i]);
    REQUIRE(correct_data[i] == copy_constructed[i]);
    REQUIRE(correct_data[i] == ibo_data[i]);
    REQUIRE(correct_data[i] == copy_constructed_data[i]);
    REQUIRE(correct_data[i] == copy_assigned[i]);
    REQUIRE(correct_data[i] == copy_assigned_data[i]);
  }

  SECTION("changing one value in gpu memory") {
    ibo[0] = 10;
    REQUIRE(ibo[0] == 10);
    REQUIRE(copy_constructed[0] == 0);
  }

  IndexBuffer move_constructed = std::move(ibo);

  REQUIRE(ibo.size() == 0);
  REQUIRE(ibo.capacity() == 0);
  REQUIRE(ibo.will_be_deleted() == false);
  REQUIRE(ibo.gl_handle() == 0);
  REQUIRE(move_constructed.gl_handle() == ibo_handle);

  REQUIRE(move_constructed[0] == 10);
  REQUIRE(move_constructed[1] == 1);
  REQUIRE(move_constructed[2] == 2);

  copy_constructed.upload_data({5, 6});
  REQUIRE(copy_constructed.capacity() == 3);
  REQUIRE(copy_constructed.size() == 2);

  REQUIRE(copy_constructed[0] == 5);
  REQUIRE(copy_constructed[1] == 6);

  move_constructed.push_back(20);
  REQUIRE(move_constructed[0] == 10);
  REQUIRE(move_constructed[1] == 1);
  REQUIRE(move_constructed[2] == 2);
  REQUIRE(move_constructed[3] == 20);
}

//------------------------------------------------------------------------------

TEST_CASE("VBO download") {
  using vbo_t = VertexBuffer<vec2<float>, vec3<float>>;
  vbo_t vbo{{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};

  auto down = vbo.download_data();
  REQUIRE(std::get<0>(down[0])[0] == 1);
  REQUIRE(std::get<0>(down[0])[1] == 2);
  REQUIRE(std::get<1>(down[0])[0] == 3);
  REQUIRE(std::get<1>(down[0])[1] == 4);
  REQUIRE(std::get<0>(down[1])[0] == 5);
  REQUIRE(std::get<0>(down[1])[1] == 6);
  REQUIRE(std::get<1>(down[1])[0] == 7);
  REQUIRE(std::get<1>(down[1])[1] == 8);
}

//------------------------------------------------------------------------------

TEST_CASE("VBO element access") {
  using vbo_t = VertexBuffer<vec3<float>>;
  vbo_t vbo{{{1, 2, 3}}, {{4, 5, 6}}};

  vbo_t::data_t front   = vbo.front();
  vbo_t::data_t back    = vbo.back();
  auto          v_front = std::get<0>(front);
  auto          v_back  = std::get<0>(back);

  REQUIRE(v_front[0] == 1);
  REQUIRE(v_front[1] == 2);
  REQUIRE(v_front[2] == 3);
  REQUIRE(v_front[3] == 4);
  REQUIRE(v_front[4] == 5);
  REQUIRE(v_front[5] == 6);
}
