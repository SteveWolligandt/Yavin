#include <tuple>
#include <yavin>

using namespace Yavin;

int main(int argc, char** argv) {
  Window w("VBO-Test", 100, 50, 3, 3);

  VertexBuffer<vec3> original{vec3{1, 2, 3}, vec3{4, 5, 6}, vec3{7, 8, 9}};
  IndexBuffer        original_ibo{0, 1, 2};

  auto copy     = original;
  auto copy_ibo = original_ibo;

  original.download_data();
  copy.download_data();
  copy_ibo.download_data();

  assert(original.cpu_size() == copy.cpu_size());

  for (size_t i = 0; i < original.cpu_size(); ++i) {
    std::cout << "original: " << std::get<0>(original[i]).x << ", "
              << std::get<0>(original[i]).y << ", "
              << std::get<0>(original[i]).z << '\n';

    std::cout << "copy    : " << std::get<0>(copy[i]).x << ", "
              << std::get<0>(copy[i]).y << ", " << std::get<0>(copy[i]).z
              << '\n';

    std::cout << "ibo     : " << copy_ibo[i] << '\n';

    assert(std::get<0>(original[i]).x == std::get<0>(copy[i]).x);
    assert(std::get<0>(original[i]).y == std::get<0>(copy[i]).y);
    assert(std::get<0>(original[i]).z == std::get<0>(copy[i]).z);
  }
  std::cin.get();
}
