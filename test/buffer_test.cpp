#include <catch.hpp>
#include <yavin>

using namespace Yavin;

Window w("Buffer Test", 100, 50);

TEST_CASE("Index Buffer") {
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

  std::cout << move_constructed.capacity() << '\n';
}

TEST_CASE("VBO") {
  VertexBuffer<vec3> vbo{vec3{1, 2, 3}, vec3{4, 5, 6}};
  auto               copy = vbo;

  auto front_copy = vbo.front();
  auto mem        = reinterpret_cast<float*>(&front_copy);
  REQUIRE(mem[0] == 1);
  REQUIRE(mem[1] == 2);
  REQUIRE(mem[2] == 3);
  REQUIRE(mem[3] == 4);
  REQUIRE(mem[4] == 5);
  REQUIRE(mem[5] == 6);
}
