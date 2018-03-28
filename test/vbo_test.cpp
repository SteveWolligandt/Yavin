#include <yavin>

using namespace Yavin;
Window w("VBO-Test", 100, 50, 4, 5);

int main(int argc, char** argv) {
  VertexBuffer<vec3> original;
  original.push_back({1, 2, 3});
  original.push_back({4, 5, 6});
  original.push_back({7, 8, 9});
  original.upload_data();

  IndexBuffer original_ibo;
  original_ibo.push_back(0);
  original_ibo.push_back(1);
  original_ibo.push_back(2);
  original_ibo.upload_data();

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
}