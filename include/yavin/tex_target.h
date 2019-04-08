#ifndef __YAVIN_TEX_TARGET_H__
#define __YAVIN_TEX_TARGET_H__

//==============================================================================
namespace yavin::tex {
//==============================================================================
template <unsigned int n>
struct target;

//------------------------------------------------------------------------------
template <unsigned int n>
static constexpr auto target_v = target<n>::value;

//------------------------------------------------------------------------------
template <>
struct target<1> {
  static constexpr GLenum value = GL_TEXTURE_1D;
};

//------------------------------------------------------------------------------
template <>
struct target<2> {
  static constexpr GLenum value = GL_TEXTURE_2D;
};

//------------------------------------------------------------------------------
template <>
struct target<3> {
  static constexpr GLenum value = GL_TEXTURE_3D;
};

//==============================================================================
}  // namespace yavin::tex
//==============================================================================

#endif
