#ifndef __YAVIN_TEX_COMPONENTS_H__
#define __YAVIN_TEX_COMPONENTS_H__

#include <cstdint>

//==============================================================================
namespace yavin {
//==============================================================================

struct R {
  static constexpr std::size_t num_components = 1;
};
struct RG {
  static constexpr std::size_t num_components = 2;
};
struct RGB {
  static constexpr std::size_t num_components = 3;
};
struct RGBA {
  static constexpr std::size_t num_components = 4;
};
struct BGR {
  static constexpr std::size_t num_components = 3;
};
struct BGRA {
  static constexpr std::size_t num_components = 4;
};
struct Depth {
  static constexpr std::size_t num_components = 1;
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
