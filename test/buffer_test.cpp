#include <catch.hpp>
#include <yavin>

using namespace Yavin;

Window                    w("Buffer Test", 100, 50);
std::vector<unsigned int> correct_ibo_data{2, 1, 4, 0};

//------------------------------------------------------------------------------

TEST_CASE("IBO download") {
  IndexBuffer ibo{2, 1, 4, 0};

  auto down = ibo.download_data();
  for (size_t i = 0; i < 4; ++i) REQUIRE(correct_ibo_data[i] == ibo[i]);
}

//------------------------------------------------------------------------------

TEST_CASE("IBO element access") {
  IndexBuffer ibo{2, 1, 4, 0};

  for (size_t i = 0; i < 4; ++i) REQUIRE(correct_ibo_data[i] == ibo[i]);

  ibo[0] = 3;
  REQUIRE(ibo[0] == 3);
  for (size_t i = 1; i < 4; ++i) REQUIRE(correct_ibo_data[i] == ibo[i]);
}

//------------------------------------------------------------------------------

TEST_CASE("IBO assignment") {
  IndexBuffer ibo{2, 1, 4, 0};
  IndexBuffer assigned{0, 1, 3, 4, 6, 7};  // some initializer data
  assigned = ibo;

  REQUIRE(ibo.size() == 4);
  REQUIRE(ibo.capacity() == 4);

  REQUIRE(assigned.size() == 4);
  REQUIRE(assigned.capacity() == 6);

  for (size_t i = 0; i < 4; ++i) {
    REQUIRE(correct_ibo_data[i] == ibo[i]);
    REQUIRE(correct_ibo_data[i] == assigned[i]);
  }
}

//------------------------------------------------------------------------------

TEST_CASE("IBO copy constructor") {
  IndexBuffer ibo{2, 1, 4, 0};
  IndexBuffer copy(ibo);

  REQUIRE(ibo.size() == 4);
  REQUIRE(ibo.capacity() == 4);

  REQUIRE(copy.size() == 4);
  REQUIRE(copy.capacity() == 4);

  for (size_t i = 0; i < 4; ++i) {
    REQUIRE(correct_ibo_data[i] == ibo[i]);
    REQUIRE(correct_ibo_data[i] == copy[i]);
  }

  ibo[0] = 10;
  REQUIRE(ibo[0] == 10);
  REQUIRE(copy[0] == 2);
}

//------------------------------------------------------------------------------

TEST_CASE("IBO move assignment") {
  IndexBuffer ibo{2, 1, 4, 0};
  auto        ibo_handle = ibo.gl_handle();

  REQUIRE(ibo.size() == 4);
  REQUIRE(ibo.capacity() == 4);

  IndexBuffer moved = std::move(ibo);

  REQUIRE(ibo.size() == 0);
  REQUIRE(ibo.capacity() == 0);

  REQUIRE(moved.size() == 4);
  REQUIRE(moved.capacity() == 4);

  REQUIRE(ibo.size() == 0);
  REQUIRE(ibo.capacity() == 0);
  REQUIRE(ibo.will_be_deleted() == false);
  REQUIRE(ibo.gl_handle() == 0);
  REQUIRE(moved.gl_handle() == ibo_handle);

  for (size_t i = 0; i < 4; ++i) REQUIRE(correct_ibo_data[i] == moved[i]);
}

//------------------------------------------------------------------------------

TEST_CASE("IBO push pop") {
  IndexBuffer ibo{2, 1, 4, 0};

  REQUIRE(ibo.size() == 4);
  REQUIRE(ibo.capacity() == 4);

  ibo.push_back(3);

  REQUIRE(ibo.size() == 5);
  REQUIRE(ibo.capacity() == 8);

  ibo.push_back(8);

  REQUIRE(ibo.size() == 6);
  REQUIRE(ibo.capacity() == 8);

  for (size_t i = 0; i < 4; ++i) REQUIRE(correct_ibo_data[i] == ibo[i]);
  REQUIRE(ibo[4] == 3);
  REQUIRE(ibo[5] == 8);
}

//------------------------------------------------------------------------------

TEST_CASE("VBO download") {
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

//------------------------------------------------------------------------------

TEST_CASE("VBO element access") {
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
