#ifndef __YAVIN_TEX_COMPONENTS_H__
#define __YAVIN_TEX_COMPONENTS_H__

//==============================================================================
namespace yavin {
//==============================================================================

struct R {
  static constexpr size_t num_components = 1;
};
struct RG {
  static constexpr size_t num_components = 2;
};
struct RGB {
  static constexpr size_t num_components = 3;
};
struct RGBA {
  static constexpr size_t num_components = 4;
};
struct BGR {
  static constexpr size_t num_components = 3;
};
struct BGRA {
  static constexpr size_t num_components = 4;
};
struct Depth {
  static constexpr size_t num_components = 1;
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
