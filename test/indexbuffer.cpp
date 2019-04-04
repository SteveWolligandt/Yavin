#include <catch2/catch.hpp>
#include <yavin>

//==============================================================================
namespace yavin::test {
//==============================================================================
extern Window window;
struct IndexBufferFixture : IndexBuffer {
  IndexBufferFixture() : IndexBuffer{2, 1, 4, 0} {}
  static constexpr std::array<unsigned int, 4> correct_ibo_data{2, 1, 4, 0};
};

//==============================================================================
TEST_CASE("[IBO] default construction") {
  IndexBuffer ibo;
  REQUIRE(ibo.gl_handle() != 0);
  REQUIRE(ibo.size() == 0);
  REQUIRE(ibo.capacity() == 0);
}

//==============================================================================
TEST_CASE_METHOD(IndexBufferFixture, "[IBO] Download") {
  auto down = download_data();
  for (size_t i = 0; i < 4; ++i) REQUIRE(correct_ibo_data[i] == at(i));
}

//==============================================================================
TEST_CASE_METHOD(IndexBufferFixture, "[IBO] element access") {
  for (size_t i = 0; i < 4; ++i) REQUIRE(correct_ibo_data[i] == at(i));

  at(0) = 3;
  REQUIRE(at(0) == 3);
  for (size_t i = 1; i < 4; ++i) REQUIRE(correct_ibo_data[i] == at(i));
}

//==============================================================================
TEST_CASE_METHOD(IndexBufferFixture, "[IBO] assignment") {
  IndexBuffer assigned{0, 1, 3, 4, 6, 7};  // some initializer data
  assigned = *this;

  REQUIRE(size() == 4);
  REQUIRE(capacity() == 4);

  REQUIRE(assigned.size() == 4);
  REQUIRE(assigned.capacity() == 6);

  for (size_t i = 0; i < 4; ++i) {
    REQUIRE(correct_ibo_data[i] == at(i));
    REQUIRE(correct_ibo_data[i] == assigned[i]);
  }
}

//==============================================================================
TEST_CASE_METHOD(IndexBufferFixture, "[IBO] copy constructor") {
  IndexBuffer copy(*this);

  REQUIRE(size() == 4);
  REQUIRE(capacity() == 4);

  REQUIRE(copy.size() == 4);
  REQUIRE(copy.capacity() == 4);

  for (size_t i = 0; i < 4; ++i) {
    REQUIRE(correct_ibo_data[i] == at(i));
    REQUIRE(correct_ibo_data[i] == copy[i]);
  }

  at(0) = 10;
  REQUIRE(at(0) == 10);
  REQUIRE(copy[0] == 2);
}

//==============================================================================
TEST_CASE_METHOD(IndexBufferFixture, "[IBO] move constructor") {
  auto ibo_handle = gl_handle();

  REQUIRE(size() == 4);
  REQUIRE(capacity() == 4);

  IndexBuffer moved = std::move(*this);

  REQUIRE(size() == 0);
  REQUIRE(capacity() == 0);

  REQUIRE(moved.size() == 4);
  REQUIRE(moved.capacity() == 4);

  REQUIRE(size() == 0);
  REQUIRE(capacity() == 0);
  REQUIRE(gl_handle() == 0);
  REQUIRE(moved.gl_handle() == ibo_handle);

  for (size_t i = 0; i < 4; ++i) REQUIRE(correct_ibo_data[i] == moved[i]);
}

//==============================================================================
TEST_CASE_METHOD(IndexBufferFixture, "[IBO] move assignement") {
  IndexBuffer moved{0, 1, 3, 4, 6, 7};  // some initializer data
  moved = std::move(*this);

  REQUIRE(size() == 6);
  REQUIRE(capacity() == 6);

  REQUIRE(moved.size() == 4);
  REQUIRE(moved.capacity() == 4);

  for (size_t i = 0; i < 4; ++i) REQUIRE(correct_ibo_data[i] == moved[i]);

  REQUIRE(at(0) == 0);
  REQUIRE(at(1) == 1);
  REQUIRE(at(2) == 3);
  REQUIRE(at(3) == 4);
  REQUIRE(at(4) == 6);
  REQUIRE(at(5) == 7);
}

//==============================================================================
TEST_CASE("[IBO] size construction") {
  SECTION("default initial") {
    IndexBuffer ibo(10);
    REQUIRE(ibo.size() == 10);
    REQUIRE(ibo.capacity() == 10);
  }
  SECTION("specified initial") {
    IndexBuffer ibo(10, 1);
    REQUIRE(ibo.size() == 10);
    REQUIRE(ibo.capacity() == 10);
    auto data = ibo.download_data();
    for (const auto& d : data) REQUIRE(d == 1);
  }
}

//==============================================================================
TEST_CASE("[IBO] std::vector construction") {
  std::vector<unsigned int> data{1, 2, 3, 4};
  IndexBuffer               ibo(data);
  REQUIRE(data.size() == ibo.size());
  REQUIRE(data.size() == ibo.capacity());
  for (size_t i = 0; i < data.size(); ++i) REQUIRE(ibo[i] == data[i]);
}

//==============================================================================
TEST_CASE_METHOD(IndexBufferFixture, "[IBO] push pop") {
  REQUIRE(size() == 4);
  REQUIRE(capacity() == 4);

  push_back(3);

  REQUIRE(size() == 5);
  REQUIRE(capacity() == 8);

  push_back(8);

  REQUIRE(size() == 6);
  REQUIRE(capacity() == 8);

  for (size_t i = 0; i < 4; ++i) REQUIRE(correct_ibo_data[i] == at(i));
  REQUIRE(at(4) == 3);
  REQUIRE(at(5) == 8);

  pop_back();
  REQUIRE(size() == 5);
  REQUIRE(capacity() == 8);
}

//==============================================================================
}  // namespace yavin::test
//==============================================================================
