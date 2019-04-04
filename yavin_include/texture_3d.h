#ifndef __YAVIN_TEXTURE3D_H__
#define __YAVIN_TEXTURE3D_H__

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
class Texture3D : public BaseTexture<GL_TEXTURE_3D, type, components> {
 public:
  //============================================================================
  using this_t                             = Texture3D<type, components>;
  using parent_t = BaseTexture<GL_TEXTURE_3D, type, components>;

  static constexpr auto default_interpolation = parent_t::default_interpolation;
  static constexpr auto default_wrap_mode     = parent_t::default_wrap_mode;
  static constexpr auto num_components        = parent_t::num_components;
  static constexpr auto gl_internal_format    = parent_t::gl_internal_format;
  static constexpr auto gl_format             = parent_t::gl_format;
  static constexpr auto gl_type               = parent_t::gl_type;

 protected:
  //============================================================================
  size_t m_width, m_height, m_depth;

 public:
  //============================================================================
  Texture3D(tex::InterpolationMode interp_mode_min,
            tex::InterpolationMode interp_mode_mag, tex::WrapMode wrap_mode_s,
            tex::WrapMode wrap_mode_t, tex::WrapMode wrap_mode_r)
      : m_width{}, m_height{}, m_depth{} {
    set_interpolation_mode_min(interp_mode_min);
    set_interpolation_mode_mag(interp_mode_mag);
    set_wrap_mode_s(wrap_mode_s);
    set_wrap_mode_t(wrap_mode_t);
    set_wrap_mode_r(wrap_mode_r);
  }

  //----------------------------------------------------------------------------
  Texture3D(tex::InterpolationMode interp_mode, tex::WrapMode wrap_mode)
      : Texture3D(interp_mode, interp_mode, wrap_mode, wrap_mode, wrap_mode) {}

  //----------------------------------------------------------------------------
  Texture3D()
      : Texture3D(default_interpolation, default_interpolation,
                  default_wrap_mode, default_wrap_mode, default_wrap_mode) {}

  //----------------------------------------------------------------------------
  Texture3D(const Texture3D& other);

  //----------------------------------------------------------------------------
  Texture3D(Texture3D&& other);
  
  //----------------------------------------------------------------------------
  Texture3D(size_t width, size_t height, size_t depth,
            tex::InterpolationMode interp_mode_min,
            tex::InterpolationMode interp_mode_mag,
            tex::WrapMode wrap_mode_s,
            tex::WrapMode wrap_mode_t,
            tex::WrapMode wrap_mode_r);

  //----------------------------------------------------------------------------
  Texture3D(size_t width, size_t height, size_t depth)
      : Texture3D(width, height, depth,
                  default_interpolation, default_interpolation,
                  default_wrap_mode, default_wrap_mode, default_wrap_mode) {}

  //----------------------------------------------------------------------------
  Texture3D(size_t width, size_t height, size_t depth,
            tex::InterpolationMode interp_mode, tex::WrapMode wrap_mode)
      : Texture3D(width, height, depth,
                  interp_mode, interp_mode,
                  wrap_mode, wrap_mode, wrap_mode) {}

  //----------------------------------------------------------------------------
  template <typename S>
  Texture3D(size_t width, size_t height, size_t depth,
            const std::vector<S>& data,
            tex::InterpolationMode interp_mode_min,
            tex::InterpolationMode interp_mode_mag,
            tex::WrapMode wrap_mode_s,
            tex::WrapMode wrap_mode_t,
            tex::WrapMode wrap_mode_r);

  //----------------------------------------------------------------------------
  template <typename S>
  Texture3D(size_t width, size_t height, size_t depth,
            const std::vector<S>& data)
      : Texture3D(width, height, depth, data,
                  default_interpolation, default_interpolation,
                  default_wrap_mode, default_wrap_mode, default_wrap_mode) {
  }

  //----------------------------------------------------------------------------
  template <typename S>
  Texture3D(size_t width, size_t height, size_t depth,
            const std::vector<S>& data,
            tex::InterpolationMode interp_mode,
            tex::WrapMode wrap_mode)
      : Texture3D(width, height, depth, data,
                  interp_mode, interp_mode,
                  wrap_mode, wrap_mode, wrap_mode) {}

  //----------------------------------------------------------------------------
  void        bind(GLuint i = 0) const;
  static void unbind(GLuint i = 0);

  //----------------------------------------------------------------------------
  void        bind_image_texture(unsigned int i = 0) const;
  static void unbind_image_texture(unsigned int i = 0);

  //----------------------------------------------------------------------------
  void set_interpolation_mode(tex::InterpolationMode mode);
  void set_interpolation_mode_min(tex::InterpolationMode mode);
  void set_interpolation_mode_mag(tex::InterpolationMode mode);

  //----------------------------------------------------------------------------
  void set_wrap_mode(tex::WrapMode mode);
  void set_wrap_mode_s(tex::WrapMode mode);
  void set_wrap_mode_t(tex::WrapMode mode);
  void set_wrap_mode_r(tex::WrapMode mode);

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
  auto height() const { return m_height; }
  auto depth() const { return m_depth; }

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
  void           upload_data(size_t width, size_t height, size_t depth,
                             const std::vector<S>& data);

  //----------------------------------------------------------------------------
  std::vector<type> download_data() const;

  //----------------------------------------------------------------------------
  void resize(size_t w, size_t h, size_t d);

  //----------------------------------------------------------------------------
  void swap(Texture3D& other) {
    parent_t::swap(other);
    std::swap(m_width, other.m_width);
    std::swap(m_height, other.m_height);
    std::swap(m_depth, other.m_depth);
  }
};

//------------------------------------------------------------------------------
template <typename type, typename components>
Texture3D<type, components>::Texture3D(const Texture3D& other)
    : m_width{}, m_height{}, m_depth{} {
  gl::copy_image_sub_data(other.m_id, GL_TEXTURE_3D, 0, 0, 0, 0, this->m_id,
                          GL_TEXTURE_3D, 0, 0, 0, 0, other.m_width,
                          other.m_height, other.m_depth);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
Texture3D<type, components>::Texture3D(Texture3D&& other)
    : parent_t{std::move(other)},
      m_width{std::exchange(other.m_width, 0)},
      m_height{std::exchange(other.m_height, 0)},
      m_depth{std::exchange(other.m_depth, 0)} {}

//------------------------------------------------------------------------------
template <typename type, typename components>
Texture3D<type, components>::Texture3D(size_t width, size_t height, size_t depth, 
                                    tex::InterpolationMode interp_mode_min,
                                    tex::InterpolationMode interp_mode_mag,
                                    tex::WrapMode          wrap_mode_s,
                                    tex::WrapMode          wrap_mode_t,
                                    tex::WrapMode          wrap_mode_r)
    : m_width{}, m_height{}, m_depth{} {
  bind();
  resize(width, height, depth);
  set_interpolation_mode_min(interp_mode_min);
  set_interpolation_mode_mag(interp_mode_mag);
  set_wrap_mode_s(wrap_mode_s);
  set_wrap_mode_t(wrap_mode_t);
  set_wrap_mode_r(wrap_mode_r);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
template <typename S>
Texture3D<type, components>::Texture3D(size_t width, size_t height, size_t depth, 
                                    const std::vector<S>&  data,
                                    tex::InterpolationMode interp_mode_min,
                                    tex::InterpolationMode interp_mode_mag,
                                    tex::WrapMode          wrap_mode_s,
                                    tex::WrapMode          wrap_mode_t,
                                    tex::WrapMode          wrap_mode_r)
    : m_width{}, m_height{}, m_depth{} {
  bind();
  upload_data(width, height, depth, data);
  set_interpolation_mode_min(interp_mode_min);
  set_interpolation_mode_mag(interp_mode_mag);
  set_wrap_mode_s(wrap_mode_s);
  set_wrap_mode_t(wrap_mode_t);
  set_wrap_mode_r(wrap_mode_r);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture3D<type, components>::bind(unsigned int i) const {
  assert(i < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
  gl::active_texture(GL_TEXTURE0 + i);
  gl::bind_texture(GL_TEXTURE_3D, this->m_id);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture3D<type, components>::unbind(unsigned int i) {
  gl::active_texture(GL_TEXTURE0 + i);
  gl::bind_texture(GL_TEXTURE_3D, 0);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture3D<type, components>::bind_image_texture(unsigned int i) const {
  gl::bind_image_texture(i, this->m_id, 0, GL_FALSE, 0, GL_READ_WRITE,
                         gl_internal_format);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture3D<type, components>::unbind_image_texture(unsigned int i) {
  gl::bind_image_texture(i, 0, 0, GL_FALSE, 0, GL_READ_WRITE, gl_internal_format);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture3D<type, components>::set_interpolation_mode(
    tex::InterpolationMode mode) {
  set_interpolation_mode_min(mode);
  set_interpolation_mode_mag(mode);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture3D<type, components>::set_interpolation_mode_min(
    tex::InterpolationMode mode) {
  gl::texture_parameter_i(this->m_id, GL_TEXTURE_MIN_FILTER, mode);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture3D<type, components>::set_interpolation_mode_mag(
    tex::InterpolationMode mode) {
  gl::texture_parameter_i(this->m_id, GL_TEXTURE_MAG_FILTER, mode);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture3D<type, components>::set_wrap_mode(tex::WrapMode mode) {
  set_wrap_mode_s(mode);
  set_wrap_mode_t(mode);
  set_wrap_mode_r(mode);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture3D<type, components>::set_wrap_mode_s(tex::WrapMode mode) {
  gl::texture_parameter_i(this->m_id, GL_TEXTURE_WRAP_S, mode);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture3D<type, components>::set_wrap_mode_t(tex::WrapMode mode) {
  gl::texture_parameter_i(this->m_id, GL_TEXTURE_WRAP_T, mode);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture3D<type, components>::set_wrap_mode_r(tex::WrapMode mode) {
  gl::texture_parameter_i(this->m_id, GL_TEXTURE_WRAP_R, mode);
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture3D<type, components>::clear(
    const std::array<type, Texture3D<type, components>::num_components>& col) {
  gl::clear_tex_image(this->m_id, 0, gl_format, gl_type, col.data());
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void Texture3D<type, components>::upload_data(const std::vector<type>& data) {
  assert(data.size() == m_width * m_height * m_depth * num_components);
  gl::tex_image_3d(GL_TEXTURE_3D, 0, gl_internal_format,
                   m_width, m_height, m_depth, 0,
                   gl_format, gl_type, data.data());
}

//------------------------------------------------------------------------------
template <typename type, typename components>
template <typename S>
void Texture3D<type, components>::upload_data(size_t w, size_t h, size_t d,
                                              const std::vector<S>& data) {
  m_width  = w;
  m_height = h;
  m_depth  = d;
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
void Texture3D<type, components>::resize(size_t w, size_t h, size_t d) {
  if (m_width != w || m_height != h || m_depth != d) {
    m_width  = w;
    m_height = h;
    m_depth  = d;
    gl::tex_image_3d(GL_TEXTURE_3D, 0, gl_internal_format, w, h, d, 0,
                     gl_format, gl_type, nullptr);
  }
}

//------------------------------------------------------------------------------
template <typename type, typename components>
std::vector<type> Texture3D<type, components>::download_data() const {
  std::vector<type> data(num_components * m_width * m_height * m_depth);
  gl::get_texture_image(this->m_id, 0, gl_format, gl_type,
                        m_width * m_height * m_depth *
                          num_components * sizeof(type),
                        data.data());
  return data;
}

//------------------------------------------------------------------------------
template <typename type, typename components>
void swap(Texture3D<type, components>& t0, Texture3D<type, components>& t1) {
  t0.swap(t1);
}

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
