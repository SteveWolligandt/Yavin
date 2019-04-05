#ifndef __YAVIN_TEXTURE1D_H__
#define __YAVIN_TEXTURE1D_H__

#include <cassert>
#include <boost/range/algorithm/copy.hpp>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include "pixel_unpack_buffer.h"
#include "texture.h"

//==============================================================================
namespace yavin {
//==============================================================================

template <typename type, typename components>
class Texture1D : public BaseTexture<GL_TEXTURE_1D, type, components> {
 public:
  //============================================================================
  using this_t                             = Texture1D<type, components>;
  using parent_t = BaseTexture<GL_TEXTURE_1D, type, components>;

  static constexpr auto default_interpolation = parent_t::default_interpolation;
  static constexpr auto default_wrap_mode     = parent_t::default_wrap_mode;
  static constexpr auto num_components        = parent_t::num_components;
  static constexpr auto gl_internal_format    = parent_t::gl_internal_format;
  static constexpr auto gl_format             = parent_t::gl_format;
  static constexpr auto gl_type               = parent_t::gl_type;

 protected:
  //============================================================================
  size_t m_width;

 public:
  //============================================================================
  Texture1D(tex::InterpolationMode interp_mode_min,
            tex::InterpolationMode interp_mode_mag, tex::WrapMode wrap_mode_s)
      : m_width{} {
    set_interpolation_mode_min(interp_mode_min);
    set_interpolation_mode_mag(interp_mode_mag);
    set_wrap_mode_s(wrap_mode_s);
  }

  //----------------------------------------------------------------------------
  Texture1D(tex::InterpolationMode interp_mode, tex::WrapMode wrap_mode)
      : Texture1D(interp_mode, interp_mode, wrap_mode) {}

  //----------------------------------------------------------------------------
  Texture1D()
      : Texture1D(default_interpolation, default_interpolation,
                  default_wrap_mode) {}

  //----------------------------------------------------------------------------
  Texture1D(const Texture1D& other);

  //----------------------------------------------------------------------------
  Texture1D(Texture1D&& other);
  
  //----------------------------------------------------------------------------
  Texture1D(size_t width,
            tex::InterpolationMode interp_mode_min,
            tex::InterpolationMode interp_mode_mag,
            tex::WrapMode wrap_mode_s);

  //----------------------------------------------------------------------------
  Texture1D(size_t width)
      : Texture1D(width,
                  default_interpolation, default_interpolation,
                  default_wrap_mode) {}

  //----------------------------------------------------------------------------
  Texture1D(size_t width,
            tex::InterpolationMode interp_mode, tex::WrapMode wrap_mode)
      : Texture1D(width,
                  interp_mode, interp_mode,
                  wrap_mode) {}

  //----------------------------------------------------------------------------
  template <typename S>
  Texture1D(size_t width,
            const std::vector<S>& data,
            tex::InterpolationMode interp_mode_min,
            tex::InterpolationMode interp_mode_mag,
            tex::WrapMode wrap_mode_s);

  //----------------------------------------------------------------------------
  template <typename S>
  Texture1D(size_t width,
            const std::vector<S>& data)
      : Texture1D(width, data,
                  default_interpolation, default_interpolation,
                  default_wrap_mode) {
  }

  //----------------------------------------------------------------------------
  template <typename S>
  Texture1D(size_t width,
            const std::vector<S>& data,
            tex::InterpolationMode interp_mode,
            tex::WrapMode wrap_mode)
      : Texture1D(width, data,
                  interp_mode, interp_mode,
                  wrap_mode) {}

  //----------------------------------------------------------------------------
  void set_interpolation_mode(tex::InterpolationMode mode);
  void set_interpolation_mode_min(tex::InterpolationMode mode);
  void set_interpolation_mode_mag(tex::InterpolationMode mode);

  //----------------------------------------------------------------------------
  void set_wrap_mode(tex::WrapMode mode);
  void set_wrap_mode_s(tex::WrapMode mode);

  //----------------------------------------------------------------------------
  template <typename _Components = components,
            typename = std::enable_if<std::is_same_v<_Components, Depth>>>
  void set_compare_func(tex::CompareFunc f) {
    gl::texture_parameter_i(this->m_id, GL_TEXTURE_COMPARE_FUNC, f);
  }

  //----------------------------------------------------------------------------
  template <typename _Components = components,
            typename = std::enable_if<std::is_same_v<_Components, Depth>>>
  void set_compare_mode(tex::CompareMode m) {
    gl::texture_parameter_i(this->m_id, GL_TEXTURE_COMPARE_MODE, m);
  }

  //----------------------------------------------------------------------------
  auto width() const { return m_width; }

  //----------------------------------------------------------------------------
  void clear(const std::array<type, num_components>& col);

  //----------------------------------------------------------------------------
  template <typename... Ts>
  void clear(Ts&&... ts) {
    static_assert(sizeof...(Ts) == num_components);
    clear({std::forward<Ts>(ts)...});
  }

 private:
  //----------------------------------------------------------------------------
  void upload_data(const std::vector<type>& data);

 public:
  //----------------------------------------------------------------------------
  template <typename S>
  void           upload_data(size_t width,
                             const std::vector<S>& data);

  //----------------------------------------------------------------------------
  std::vector<type> download_data() const;

  //----------------------------------------------------------------------------
  void resize(size_t w);

  //----------------------------------------------------------------------------
  void swap(Texture1D& other) {
    parent_t::swap(other);
    std::swap(m_width, other.m_width);
  }
};

//------------------------------------------------------------------------------
template <typename type, typename components>
Texture1D<type, components>::Texture1D(const Texture1D& other)
    : m_width{} {
  gl::copy_image_sub_data(other.m_id, GL_TEXTURE_1D, 0, 0, 0, 0, this->m_id,
                          GL_TEXTURE_1D, 0, 0, 0, 0, other.m_width,
                          1, 1);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
Texture1D<type, components>::Texture1D(Texture1D&& other)
    : parent_t{std::move(other)},
      m_width{std::exchange(other.m_width, 0)} {}

//------------------------------------------------------------------------------
template <typename type, typename components>
Texture1D<type, components>::Texture1D(size_t width, 
                                    tex::InterpolationMode interp_mode_min,
                                    tex::InterpolationMode interp_mode_mag,
                                    tex::WrapMode          wrap_mode_s)
    : m_width{} {
  resize(width);
  set_interpolation_mode_min(interp_mode_min);
  set_interpolation_mode_mag(interp_mode_mag);
  set_wrap_mode_s(wrap_mode_s);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
template <typename S>
Texture1D<type, components>::Texture1D(size_t width, 
                                    const std::vector<S>&  data,
                                    tex::InterpolationMode interp_mode_min,
                                    tex::InterpolationMode interp_mode_mag,
                                    tex::WrapMode          wrap_mode_s)
    : m_width{} {
  upload_data(width, data);
  set_interpolation_mode_min(interp_mode_min);
  set_interpolation_mode_mag(interp_mode_mag);
  set_wrap_mode_s(wrap_mode_s);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture1D<type, components>::set_interpolation_mode(
    tex::InterpolationMode mode) {
  set_interpolation_mode_min(mode);
  set_interpolation_mode_mag(mode);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture1D<type, components>::set_interpolation_mode_min(
    tex::InterpolationMode mode) {
  gl::texture_parameter_i(this->m_id, GL_TEXTURE_MIN_FILTER, mode);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture1D<type, components>::set_interpolation_mode_mag(
    tex::InterpolationMode mode) {
  gl::texture_parameter_i(this->m_id, GL_TEXTURE_MAG_FILTER, mode);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture1D<type, components>::set_wrap_mode(tex::WrapMode mode) {
  set_wrap_mode_s(mode);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture1D<type, components>::set_wrap_mode_s(tex::WrapMode mode) {
  gl::texture_parameter_i(this->m_id, GL_TEXTURE_WRAP_S, mode);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture1D<type, components>::clear(
    const std::array<type, Texture1D<type, components>::num_components>& col) {
  gl::clear_tex_image(this->m_id, 0, gl_format, gl_type, col.data());
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture1D<type, components>::upload_data(const std::vector<type>& data) {
  assert(data.size() == m_width * num_components);
  this->bind();
  gl::tex_image_1d(GL_TEXTURE_1D, 0, gl_internal_format,
                   m_width, 0,
                   gl_format, gl_type, data.data());
}

//------------------------------------------------------------------------------
template <typename type, typename components>
template <typename S>
void Texture1D<type, components>::upload_data(size_t w,
                                              const std::vector<S>& data) {
  m_width  = w;
  if constexpr (std::is_same_v<S, type>)
    upload_data(data);
  else {
    std::vector<type> converted_data(data.size());
    boost::copy(data, begin(converted_data));
    upload_data(converted_data);
  }
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture1D<type, components>::resize(size_t w) {
  if (m_width != w) {
    m_width  = w;
    this->bind();
    gl::tex_image_1d(GL_TEXTURE_1D, 0, gl_internal_format, w, 0,
                     gl_format, gl_type, nullptr);
  }
}

//------------------------------------------------------------------------------
template <typename type, typename components>
std::vector<type> Texture1D<type, components>::download_data() const {
  std::vector<type> data(num_components * m_width);
  gl::get_texture_image(this->m_id, 0, gl_format, gl_type,
                        m_width *
                          num_components * sizeof(type),
                        data.data());
  return data;
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void swap(Texture1D<type, components>& t0, Texture1D<type, components>& t1) {
  t0.swap(t1);
}

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
