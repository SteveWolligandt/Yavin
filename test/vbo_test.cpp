#include <yavin>

using namespace Yavin;

int main(int argc, char** argv) {
  Window w("VBO-Test", 100, 50, 4, 5);

  VertexBuffer<vec3> original;
  original.push_back({1, 2, 3});
  original.push_back({4, 5, 6});
  original.push_back({7, 8, 9});
  original.upload_data();

  VertexBuffer<vec3> copy = original;

  original.download_data();
  copy.download_data();

  assert(original.cpu_size() == copy.cpu_size());

  for (size_t i = 0; i < original.cpu_size(); ++i) {
    std::cout << "original: " << std::get<0>(original[i]).x << ", "
              << std::get<0>(original[i]).y << ", "
              << std::get<0>(original[i]).z << '\n';
    std::cout << "copy    : " << std::get<0>(copy[i]).x << ", "
              << std::get<0>(copy[i]).y << ", " << std::get<0>(copy[i]).z
              << '\n';
    assert(std::get<0>(original[i]).x == std::get<0>(copy[i]).x);
    assert(std::get<0>(original[i]).y == std::get<0>(copy[i]).y);
    assert(std::get<0>(original[i]).z == std::get<0>(copy[i]).z);
  }
}
