#ifndef __YAVIN_TEXTURE2D_H__
#define __YAVIN_TEXTURE2D_H__

#include <cassert>
#include <iostream>
#include <png++/png.hpp>
#include <string>
#include <type_traits>
#include <vector>
#include "pixel_unpack_buffer.h"
#include "tex_helper.h"
#include "texture.h"

//==============================================================================
namespace yavin {
//==============================================================================

template <typename T, typename Components>
class Texture2D : public BaseTexture<GL_TEXTURE_2D> {
 public:
  using type                     = T;
  using components               = Components;
  using this_t                   = Texture2D<type, components>;
  using parent_t                 = BaseTexture<GL_TEXTURE_2D>;
  static constexpr bool is_float = std::is_same<float, type>::value;
  static constexpr auto default_interpolation = tex::LINEAR;
  static constexpr auto default_wrap_mode     = tex::REPEAT;
  template <typename C>
  static constexpr bool is_loadable =
      std::is_same_v<C, R> || std::is_same_v<C, RGB> ||
      std::is_same_v<C, RGBA> || std::is_same_v<C, BGR> ||
      std::is_same_v<C, BGRA>;

  static constexpr auto num_components = components::num_components;
  static constexpr auto gl_internal_format =
      TexHelper::comb<type, components>::internal_format;
  static constexpr auto gl_format = TexHelper::comb<type, components>::format;
  static constexpr auto gl_type   = TexHelper::comb<type, components>::type;

  Texture2D(const Texture2D& other);
  Texture2D(Texture2D&& other);

  template <typename c = components,
            typename   = std::enable_if_t<is_loadable<c>>>
  Texture2D(const std::string& filepath)
      : Texture2D(filepath, default_interpolation, default_interpolation,
                  default_wrap_mode, default_wrap_mode) {}
  template <typename c = components,
            typename   = std::enable_if_t<is_loadable<c>>>
  Texture2D(const std::string& filepath, tex::InterpolationMode interp_mode,
            tex::WrapMode wrap_mode)
      : Texture2D(filepath, interp_mode, interp_mode, wrap_mode, wrap_mode) {}
  template <typename c = components,
            typename   = std::enable_if_t<is_loadable<c>>>
  Texture2D(const std::string& filepath, tex::InterpolationMode interp_mode_min,
            tex::InterpolationMode interp_mode_mag, tex::WrapMode wrap_mode_s,
            tex::WrapMode wrap_mode_t);

  Texture2D(unsigned int width, unsigned int height)
      : Texture2D(width, height, default_interpolation, default_interpolation,
                  default_wrap_mode, default_wrap_mode) {}
  Texture2D(unsigned int width, unsigned int height,
            tex::InterpolationMode interp_mode, tex::WrapMode wrap_mode)
      : Texture2D(width, height, interp_mode, interp_mode, wrap_mode,
                  wrap_mode) {}
  Texture2D(unsigned int width, unsigned int height,
            tex::InterpolationMode interp_mode_min,
            tex::InterpolationMode interp_mode_mag, tex::WrapMode wrap_mode_s,
            tex::WrapMode wrap_mode_t);

  template <typename S>
  Texture2D(unsigned int width, unsigned int height, const std::vector<S>& data)
      : Texture2D(width, height, data, default_interpolation,
                  default_interpolation, default_wrap_mode, default_wrap_mode) {
  }
  template <typename S>
  Texture2D(unsigned int width, unsigned int height, const std::vector<S>& data,
            tex::InterpolationMode interp_mode, tex::WrapMode wrap_mode)
      : Texture2D(width, height, data, interp_mode, interp_mode, wrap_mode,
                  wrap_mode) {}

  template <typename S>
  Texture2D(unsigned int width, unsigned int height, const std::vector<S>& data,
            tex::InterpolationMode interp_mode_min,
            tex::InterpolationMode interp_mode_mag, tex::WrapMode wrap_mode_s,
            tex::WrapMode wrap_mode_t);

  void        bind(unsigned int i = 0) const;
  static void unbind(unsigned int i = 0);
  void        bind_image_texture(unsigned int i = 0) const;
  static void unbind_image_texture(unsigned int i = 0);

  void set_interpolation_mode(tex::InterpolationMode mode);
  void set_interpolation_mode_min(tex::InterpolationMode mode);
  void set_interpolation_mode_mag(tex::InterpolationMode mode);

  void set_wrap_mode(tex::WrapMode mode);
  void set_wrap_mode_s(tex::WrapMode mode);
  void set_wrap_mode_t(tex::WrapMode mode);

  template <typename _Components = components,
            typename = std::enable_if<std::is_same_v<_Components, Depth>>>
  void set_compare_func(tex::CompareFunc f) {
    gl::texture_parameter_i(this->m_id, GL_TEXTURE_COMPARE_FUNC, f);
  }

  template <typename _Components = components,
            typename = std::enable_if<std::is_same_v<_Components, Depth>>>
  void set_compare_mode(tex::CompareMode m) {
    gl::texture_parameter_i(this->m_id, GL_TEXTURE_COMPARE_MODE, m);
  }

  auto width() const;
  auto height() const;

  void clear(const std::array<type, num_components>& col);

  template <typename... Ts>
  void clear(Ts&&... ts) {
    static_assert(sizeof...(Ts) == num_components);
    clear({std::forward<Ts>(ts)...});
  }

  template <typename c = components,
            typename   = std::enable_if_t<is_loadable<c>>>
  void load_png(const std::string& filepath);
  template <typename c = components,
            typename   = std::enable_if_t<is_loadable<c>>>
  void save_png(const std::string& filepath, type scale_factor = 1);

 private:
  void upload_data(const std::vector<type>& data);

 public:
  template <typename S>
  void           upload_data(unsigned int width, unsigned int height,
                             const std::vector<S>& data);
  std::vector<type> download_data() const;
  void           set_data(const PixelUnpackBuffer<type>& pbo);

  void resize(unsigned int w, unsigned int h);
  void swap(Texture2D& other) {
    parent_t::swap(other);
    std::swap(m_width, other.m_width);
    std::swap(m_height, other.m_height);
  }

 protected:
  unsigned int m_width, m_height;
};

template <typename type, typename tex_t>
struct tex_png;
template <typename type>
struct tex_png<type, R> {
  static constexpr size_t num_components        = R::num_components;
  using png_t                       = png::image<png::rgb_pixel>;
  static constexpr auto load_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<type>(image[y - 1][x].red));
    data.push_back(static_cast<type>(image[y - 1][x].green));
    data.push_back(static_cast<type>(image[y - 1][x].blue));
  };
  static constexpr auto save_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y, size_t idx,
                                        float scale_factor) {
    if constexpr (std::is_same_v<float, type>) {
      image[image.get_height() - 1 - y][x].red =
          data[idx * num_components] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components] * 255.0f * scale_factor;
    } else {
      image[image.get_height() - 1 - y][x].red   = data[idx * num_components] * scale_factor;
      image[image.get_height() - 1 - y][x].green = data[idx * num_components] * scale_factor;
      image[image.get_height() - 1 - y][x].blue  = data[idx * num_components] * scale_factor;
    }
  };
};
template <typename type>
struct tex_png<type, RGB> {
  static constexpr size_t num_components        = RGB::num_components;
  using png_t                       = png::image<png::rgb_pixel>;
  static constexpr auto load_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<type>(image[y - 1][x].red));
    data.push_back(static_cast<type>(image[y - 1][x].green));
    data.push_back(static_cast<type>(image[y - 1][x].blue));
  };
  static constexpr auto save_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y, size_t idx,
                                        float scale_factor) {
    if constexpr (std::is_same_v<float, type>) {
      image[image.get_height() - 1 - y][x].red =
          data[idx * num_components] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components + 2] * 255.0f * scale_factor;
    } else {
      image[image.get_height() - 1 - y][x].red = data[idx * num_components] * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1] * scale_factor;
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components + 2] * scale_factor;
    }
  };
};
template <typename type>
struct tex_png<type, BGR> {
  static constexpr size_t num_components        = BGR::num_components;
  using png_t                       = png::image<png::rgb_pixel>;
  static constexpr auto load_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<type>(image[y - 1][x].blue));
    data.push_back(static_cast<type>(image[y - 1][x].green));
    data.push_back(static_cast<type>(image[y - 1][x].red));
  };
  static constexpr auto save_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y, size_t idx,
                                        float scale_factor) {
    if constexpr (std::is_same_v<float, type>) {
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].red =
          data[idx * num_components + 2] * 255.0f * scale_factor;
    } else {
      image[image.get_height() - 1 - y][x].blue = data[idx * num_components] * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1] * scale_factor;
      image[image.get_height() - 1 - y][x].red =
          data[idx * num_components + 2] * scale_factor;
    }
  };
};
template <typename type>
struct tex_png<type, RGBA> {
  static constexpr size_t num_components        = RGBA::num_components;
  using png_t                       = png::image<png::rgba_pixel>;
  static constexpr auto load_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<type>(image[y - 1][x].red));
    data.push_back(static_cast<type>(image[y - 1][x].green));
    data.push_back(static_cast<type>(image[y - 1][x].blue));
    data.push_back(static_cast<type>(image[y - 1][x].alpha));
  };
  static constexpr auto save_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y, size_t idx,
                                        float scale_factor) {
    if constexpr (std::is_same_v<float, type>) {
      image[image.get_height() - 1 - y][x].red =
          data[idx * num_components] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components + 2] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].alpha =
          data[idx * num_components + 3] * 255.0f * scale_factor;
    } else {
      image[image.get_height() - 1 - y][x].red = data[idx * num_components] * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1] * scale_factor;
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components + 2] * scale_factor;
      image[image.get_height() - 1 - y][x].alpha =
          data[idx * num_components + 3] * scale_factor;
    }
  };
};
template <typename type>
struct tex_png<type, BGRA> {
  static constexpr size_t num_components        = BGRA::num_components;
  using png_t                       = png::image<png::rgba_pixel>;
  static constexpr auto load_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<type>(image[y - 1][x].blue));
    data.push_back(static_cast<type>(image[y - 1][x].green));
    data.push_back(static_cast<type>(image[y - 1][x].red));
    data.push_back(static_cast<type>(image[y - 1][x].alpha));
  };
  static constexpr auto save_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y, size_t idx,
                                        float scale_factor) {
    if constexpr (std::is_same_v<float, type>) {
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].red =
          data[idx * num_components + 2] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].alpha =
          data[idx * num_components + 3] * 255.0f * scale_factor;
    } else {
      image[image.get_height() - 1 - y][x].blue = data[idx * num_components] * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1] * scale_factor;
      image[image.get_height() - 1 - y][x].red =
          data[idx * num_components + 2] * scale_factor;
      image[image.get_height() - 1 - y][x].alpha =
          data[idx * num_components + 3] * scale_factor;
    }
  };
};

template <typename type, typename components>
template <typename, typename>
Texture2D<type, components>::Texture2D(const std::string&     filepath,
                                    tex::InterpolationMode interp_mode_min,
                                    tex::InterpolationMode interp_mode_mag,
                                    tex::WrapMode          wrap_mode_s,
                                    tex::WrapMode          wrap_mode_t) {
  assert(filepath.substr(filepath.size() - 3, 3) == "png");
  bind();
  load_png(filepath);
  set_interpolation_mode_min(interp_mode_min);
  set_interpolation_mode_mag(interp_mode_mag);
  set_wrap_mode_s(wrap_mode_s);
  set_wrap_mode_t(wrap_mode_t);
}

template <typename type, typename components>
Texture2D<type, components>::Texture2D(unsigned int width, unsigned int height,
                                    tex::InterpolationMode interp_mode_min,
                                    tex::InterpolationMode interp_mode_mag,
                                    tex::WrapMode          wrap_mode_s,
                                    tex::WrapMode          wrap_mode_t)
    : m_width(0), m_height(0) {
  bind();
  resize(width, height);
  set_interpolation_mode_min(interp_mode_min);
  set_interpolation_mode_mag(interp_mode_mag);
  set_wrap_mode_s(wrap_mode_s);
  set_wrap_mode_t(wrap_mode_t);
}

template <typename type, typename components>
Texture2D<type, components>::Texture2D(const Texture2D& other)
    : Texture2D(other.m_width, other.m_height) {
  gl::copy_image_sub_data(other.m_id, GL_TEXTURE_2D, 0, 0, 0, 0, this->m_id,
                          GL_TEXTURE_2D, 0, 0, 0, 0, m_width, m_height, 1);
}

template <typename type, typename components>
Texture2D<type, components>::Texture2D(Texture2D&& other)
    : parent_t(std::move(other)),
      m_width(std::exchange(other.m_width, 0)),
      m_height(std::exchange(other.m_height, 0)) {}

template <typename type, typename components>
template <typename S>
Texture2D<type, components>::Texture2D(unsigned int width, unsigned int height,
                                    const std::vector<S>&  data,
                                    tex::InterpolationMode interp_mode_min,
                                    tex::InterpolationMode interp_mode_mag,
                                    tex::WrapMode          wrap_mode_s,
                                    tex::WrapMode          wrap_mode_t)
    : m_width(width), m_height(height) {
  bind();
  upload_data(width, height, data);
  set_interpolation_mode_min(interp_mode_min);
  set_interpolation_mode_mag(interp_mode_mag);
  set_wrap_mode_s(wrap_mode_s);
  set_wrap_mode_t(wrap_mode_t);
}

template <typename type, typename components>
void Texture2D<type, components>::bind(unsigned int i) const {
  assert(i < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
  gl::active_texture(GL_TEXTURE0 + i);
  gl::bind_texture(GL_TEXTURE_2D, this->m_id);
}

template <typename type, typename components>
void Texture2D<type, components>::unbind(unsigned int i) {
  gl::active_texture(GL_TEXTURE0 + i);
  gl::bind_texture(GL_TEXTURE_2D, 0);
}

template <typename type, typename components>
void Texture2D<type, components>::bind_image_texture(unsigned int i) const {
  gl::bind_image_texture(i, this->m_id, 0, GL_FALSE, 0, GL_READ_WRITE,
                         gl_internal_format);
}

template <typename type, typename components>
void Texture2D<type, components>::unbind_image_texture(unsigned int i) {
  gl::bind_image_texture(i, 0, 0, GL_FALSE, 0, GL_READ_WRITE, gl_internal_format);
}

template <typename type, typename components>
void Texture2D<type, components>::set_interpolation_mode(
    tex::InterpolationMode mode) {
  set_interpolation_mode_min(mode);
  set_interpolation_mode_mag(mode);
}

template <typename type, typename components>
void Texture2D<type, components>::set_interpolation_mode_min(
    tex::InterpolationMode mode) {
  gl::texture_parameter_i(this->m_id, GL_TEXTURE_MIN_FILTER, mode);
}

template <typename type, typename components>
void Texture2D<type, components>::set_interpolation_mode_mag(
    tex::InterpolationMode mode) {
  gl::texture_parameter_i(this->m_id, GL_TEXTURE_MAG_FILTER, mode);
}

template <typename type, typename components>
void Texture2D<type, components>::set_wrap_mode(tex::WrapMode mode) {
  set_wrap_mode_s(mode);
  set_wrap_mode_t(mode);
}

template <typename type, typename components>
void Texture2D<type, components>::set_wrap_mode_s(tex::WrapMode mode) {
  gl::texture_parameter_i(this->m_id, GL_TEXTURE_WRAP_S, mode);
}

template <typename type, typename components>
void Texture2D<type, components>::set_wrap_mode_t(tex::WrapMode mode) {
  gl::texture_parameter_i(this->m_id, GL_TEXTURE_WRAP_T, mode);
}

template <typename type, typename components>
auto Texture2D<type, components>::width() const {
  return m_width;
}

template <typename type, typename components>
auto Texture2D<type, components>::height() const {
  return m_height;
}

template <typename type, typename components>
void Texture2D<type, components>::clear(
    const std::array<type, Texture2D<type, components>::num_components>& col) {
  gl::clear_tex_image(this->m_id, 0, gl_format, gl_type, col.data());
}

template <typename type, typename components>
template <typename, typename>
void Texture2D<type, components>::load_png(const std::string& filepath) {
  using tex_png_t = tex_png<type, components>;
  typename tex_png_t::type image;
  image.read(filepath);
  m_width  = image.get_width();
  m_height = image.get_height();
  std::vector<type> data;
  data.reserve(m_width * m_height * num_components);
  for (png::uint_32 y = image.get_height(); y > 0; --y)
    for (png::uint_32 x = 0; x < image.get_width(); ++x)
      tex_png_t::load_pixel(data, image, x, y);
  if constexpr (is_float) {
    auto normalize = [](auto d) { return d / 255.0f; };
    std::transform(begin(data), end(data), begin(data), normalize);
  }

  upload_data(data);
}

template <typename type, typename components>
template <typename, typename>
void Texture2D<type, components>::save_png(const std::string& filepath,
                                        type                  scale_factor) {
  using tex_png_t               = tex_png<type, components>;
  auto                     data = download_data();
  typename tex_png_t::type image(m_width, m_height);
  for (unsigned int y = 0; y < image.get_height(); ++y)
    for (png::uint_32 x = 0; x < image.get_width(); ++x) {
      unsigned int idx = x + m_width * y;
      tex_png_t::save_pixel(data, image, x, y, idx, scale_factor);
    }
  image.write(filepath);
}

template <typename type, typename components>
void Texture2D<type, components>::upload_data(const std::vector<type>& data) {
  gl::tex_image_2d(GL_TEXTURE_2D, 0, gl_internal_format, m_width, m_height, 0,
                   gl_format, gl_type, data.data());
}

template <typename type, typename components>
template <typename S>
void Texture2D<type, components>::upload_data(unsigned int          width,
                                           unsigned int          height,
                                           const std::vector<S>& data) {
  m_width  = width;
  m_height = height;
  if constexpr (std::is_same_v<S, type>)
    upload_data(data);
  else {
    std::vector<type> converted_data(data.size());
    std::copy(begin(data), end(data), begin(converted_data));
    upload_data(converted_data);
  }
}

template <typename type, typename components>
void Texture2D<type, components>::set_data(const PixelUnpackBuffer<type>& pbo) {
  bind();
  pbo.bind();
  gl::tex_sub_image_2d(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, gl_format, gl_type,
                       0);
  pbo.unbind();
}

template <typename type, typename components>
void Texture2D<type, components>::resize(unsigned int width, unsigned int height) {
  if (m_width != width || m_height != height) {
    m_width  = width;
    m_height = height;
    gl::tex_image_2d(GL_TEXTURE_2D, 0, gl_internal_format, m_width, m_height, 0,
                     gl_format, gl_type, nullptr);
  }
}

template <typename type, typename components>
std::vector<type> Texture2D<type, components>::download_data() const {
  std::vector<type> data(num_components * m_width * m_height);
  gl::get_texture_image(this->m_id, 0, gl_format, gl_type,
                        m_width * m_height * num_components * sizeof(type), data.data());
  return data;
}

template <typename type, typename components>
void swap(Texture2D<type, components>& t0, Texture2D<type, components>& t1) {
  t0.swap(t1);
}

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
