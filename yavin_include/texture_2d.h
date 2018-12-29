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
class Texture2D : public Texture {
 public:
  static constexpr bool              is_float = std::is_same<float, T>::value;
  static constexpr InterpolationMode default_interpolation = LINEAR;
  static constexpr WrapMode          default_wrap_mode     = REPEAT;
  template <typename C>
  static constexpr bool is_loadable =
      std::is_same_v<C, R> || std::is_same_v<C, RGB> ||
      std::is_same_v<C, RGBA> || std::is_same_v<C, BGR> ||
      std::is_same_v<C, BGRA>;

  Texture2D(const Texture2D& other);
  Texture2D(Texture2D&& other);

  template <typename c = Components,
            typename   = std::enable_if_t<is_loadable<c>>>
  Texture2D(const std::string& filepath)
      : Texture2D(filepath, default_interpolation, default_interpolation,
                  default_wrap_mode, default_wrap_mode) {}
  template <typename c = Components,
            typename   = std::enable_if_t<is_loadable<c>>>
  Texture2D(const std::string& filepath, InterpolationMode interp_mode,
            WrapMode wrap_mode)
      : Texture2D(filepath, interp_mode, interp_mode, wrap_mode, wrap_mode) {}
  template <typename c = Components,
            typename   = std::enable_if_t<is_loadable<c>>>
  Texture2D(const std::string& filepath, InterpolationMode interp_mode_min,
            InterpolationMode interp_mode_mag, WrapMode wrap_mode_s,
            WrapMode wrap_mode_t);

  Texture2D(unsigned int width, unsigned int height)
      : Texture2D(width, height, default_interpolation, default_interpolation,
                  default_wrap_mode, default_wrap_mode) {}
  Texture2D(unsigned int width, unsigned int height,
            InterpolationMode interp_mode, WrapMode wrap_mode)
      : Texture2D(width, height, interp_mode, interp_mode, wrap_mode,
                  wrap_mode) {}
  Texture2D(unsigned int width, unsigned int height,
            InterpolationMode interp_mode_min,
            InterpolationMode interp_mode_mag, WrapMode wrap_mode_s,
            WrapMode wrap_mode_t);

  Texture2D(unsigned int width, unsigned int height, const std::vector<T>& data)
      : Texture2D(width, height, data, default_interpolation,
                  default_interpolation, default_wrap_mode, default_wrap_mode) {
  }
  Texture2D(unsigned int width, unsigned int height, const std::vector<T>& data,
            InterpolationMode interp_mode, WrapMode wrap_mode)
      : Texture2D(width, height, data, interp_mode, interp_mode, wrap_mode,
                  wrap_mode) {}
  Texture2D(unsigned int width, unsigned int height, const std::vector<T>& data,
            InterpolationMode interp_mode_min = default_interpolation,
            InterpolationMode interp_mode_mag = default_interpolation,
            WrapMode          wrap_mode_s     = default_wrap_mode,
            WrapMode          wrap_mode_t     = default_wrap_mode);

  template <typename S>
  Texture2D(unsigned int width, unsigned int height, const std::vector<S>& data)
      : Texture2D(width, height, data, default_interpolation,
                  default_interpolation, default_wrap_mode, default_wrap_mode) {
  }
  template <typename S>
  Texture2D(unsigned int width, unsigned int height, const std::vector<S>& data,
            InterpolationMode interp_mode, WrapMode wrap_mode)
      : Texture2D(width, height, data, interp_mode, interp_mode, wrap_mode,
                  wrap_mode) {}

  template <typename S>
  Texture2D(unsigned int width, unsigned int height, const std::vector<S>& data,
            InterpolationMode interp_mode_min,
            InterpolationMode interp_mode_mag, WrapMode wrap_mode_s,
            WrapMode wrap_mode_t);

  void        bind(unsigned int i = 0) const;
  static void unbind(unsigned int i = 0);
  void        bind_image_texture(unsigned int i = 0) const;
  static void unbind_image_texture(unsigned int i = 0);

  void set_interpolation_mode(InterpolationMode mode);
  void set_interpolation_mode_min(InterpolationMode mode);
  void set_interpolation_mode_mag(InterpolationMode mode);

  void set_wrap_mode(WrapMode mode);
  void set_wrap_mode_s(WrapMode mode);
  void set_wrap_mode_t(WrapMode mode);

  template <typename _Components = Components,
            typename = std::enable_if<std::is_same_v<_Components, Depth>>>
  void set_compare_func(CompareFunc f) {
    gl::texture_parameter_i(m_id, GL_TEXTURE_COMPARE_FUNC, f);
  }

  template <typename _Components = Components,
            typename = std::enable_if<std::is_same_v<_Components, Depth>>>
  void set_compare_mode(CompareMode m) {
    gl::texture_parameter_i(m_id, GL_TEXTURE_COMPARE_MODE, m);
  }

  auto width() const;
  auto height() const;

  template <typename c = Components,
            typename   = std::enable_if_t<is_loadable<c>>>
  void load_png(const std::string& filepath);
  template <typename c = Components,
            typename   = std::enable_if_t<is_loadable<c>>>
  void save_png(const std::string& filepath, T scale_factor = 1);

 private:
  void upload_data(const std::vector<T>& data);

 public:
  void           upload_data(unsigned int width, unsigned int height,
                             const std::vector<T>& data);
  std::vector<T> download_data() const;
  void           set_data(const PixelUnpackBuffer<T>& pbo);

  void resize(unsigned int w, unsigned int h);

  static constexpr unsigned int n =
      TexHelper::comb<T, Components>::num_components;
  static constexpr GLint internal_format =
      TexHelper::comb<T, Components>::internal_format;
  static constexpr GLenum format = TexHelper::comb<T, Components>::format;
  static constexpr GLenum type   = TexHelper::comb<T, Components>::type;

 protected:
  unsigned int m_width, m_height;
  bool         m_is_consistent = false;
};

template <typename T, typename tex_t>
struct tex_png;
template <typename T>
struct tex_png<T, R> {
  static constexpr size_t n        = R::n;
  using type                       = png::image<png::rgb_pixel>;
  static constexpr auto load_pixel = [](std::vector<T>& data, type& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<T>(image[y - 1][x].red));
    data.push_back(static_cast<T>(image[y - 1][x].green));
    data.push_back(static_cast<T>(image[y - 1][x].blue));
  };
  static constexpr auto save_pixel = [](std::vector<T>& data, type& image,
                                        size_t x, size_t y, size_t idx,
                                        float scale_factor) {
    if constexpr (std::is_same_v<float, T>) {
      image[image.get_height() - 1 - y][x].red =
          data[idx * n] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * n] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].blue =
          data[idx * n] * 255.0f * scale_factor;
    } else {
      image[image.get_height() - 1 - y][x].red   = data[idx * n] * scale_factor;
      image[image.get_height() - 1 - y][x].green = data[idx * n] * scale_factor;
      image[image.get_height() - 1 - y][x].blue  = data[idx * n] * scale_factor;
    }
  };
};
template <typename T>
struct tex_png<T, RGB> {
  static constexpr size_t n        = RGB::n;
  using type                       = png::image<png::rgb_pixel>;
  static constexpr auto load_pixel = [](std::vector<T>& data, type& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<T>(image[y - 1][x].red));
    data.push_back(static_cast<T>(image[y - 1][x].green));
    data.push_back(static_cast<T>(image[y - 1][x].blue));
  };
  static constexpr auto save_pixel = [](std::vector<T>& data, type& image,
                                        size_t x, size_t y, size_t idx,
                                        float scale_factor) {
    if constexpr (std::is_same_v<float, T>) {
      image[image.get_height() - 1 - y][x].red =
          data[idx * n] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * n + 1] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].blue =
          data[idx * n + 2] * 255.0f * scale_factor;
    } else {
      image[image.get_height() - 1 - y][x].red = data[idx * n] * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * n + 1] * scale_factor;
      image[image.get_height() - 1 - y][x].blue =
          data[idx * n + 2] * scale_factor;
    }
  };
};
template <typename T>
struct tex_png<T, BGR> {
  static constexpr size_t n        = BGR::n;
  using type                       = png::image<png::rgb_pixel>;
  static constexpr auto load_pixel = [](std::vector<T>& data, type& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<T>(image[y - 1][x].blue));
    data.push_back(static_cast<T>(image[y - 1][x].green));
    data.push_back(static_cast<T>(image[y - 1][x].red));
  };
  static constexpr auto save_pixel = [](std::vector<T>& data, type& image,
                                        size_t x, size_t y, size_t idx,
                                        float scale_factor) {
    if constexpr (std::is_same_v<float, T>) {
      image[image.get_height() - 1 - y][x].blue =
          data[idx * n] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * n + 1] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].red =
          data[idx * n + 2] * 255.0f * scale_factor;
    } else {
      image[image.get_height() - 1 - y][x].blue = data[idx * n] * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * n + 1] * scale_factor;
      image[image.get_height() - 1 - y][x].red =
          data[idx * n + 2] * scale_factor;
    }
  };
};
template <typename T>
struct tex_png<T, RGBA> {
  static constexpr size_t n        = RGBA::n;
  using type                       = png::image<png::rgba_pixel>;
  static constexpr auto load_pixel = [](std::vector<T>& data, type& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<T>(image[y - 1][x].red));
    data.push_back(static_cast<T>(image[y - 1][x].green));
    data.push_back(static_cast<T>(image[y - 1][x].blue));
    data.push_back(static_cast<T>(image[y - 1][x].alpha));
  };
  static constexpr auto save_pixel = [](std::vector<T>& data, type& image,
                                        size_t x, size_t y, size_t idx,
                                        float scale_factor) {
    if constexpr (std::is_same_v<float, T>) {
      image[image.get_height() - 1 - y][x].red =
          data[idx * n] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * n + 1] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].blue =
          data[idx * n + 2] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].alpha =
          data[idx * n + 3] * 255.0f * scale_factor;
    } else {
      image[image.get_height() - 1 - y][x].red = data[idx * n] * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * n + 1] * scale_factor;
      image[image.get_height() - 1 - y][x].blue =
          data[idx * n + 2] * scale_factor;
      image[image.get_height() - 1 - y][x].alpha =
          data[idx * n + 3] * scale_factor;
    }
  };
};
template <typename T>
struct tex_png<T, BGRA> {
  static constexpr size_t n        = BGRA::n;
  using type                       = png::image<png::rgba_pixel>;
  static constexpr auto load_pixel = [](std::vector<T>& data, type& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<T>(image[y - 1][x].blue));
    data.push_back(static_cast<T>(image[y - 1][x].green));
    data.push_back(static_cast<T>(image[y - 1][x].red));
    data.push_back(static_cast<T>(image[y - 1][x].alpha));
  };
  static constexpr auto save_pixel = [](std::vector<T>& data, type& image,
                                        size_t x, size_t y, size_t idx,
                                        float scale_factor) {
    if constexpr (std::is_same_v<float, T>) {
      image[image.get_height() - 1 - y][x].blue =
          data[idx * n] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * n + 1] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].red =
          data[idx * n + 2] * 255.0f * scale_factor;
      image[image.get_height() - 1 - y][x].alpha =
          data[idx * n + 3] * 255.0f * scale_factor;
    } else {
      image[image.get_height() - 1 - y][x].blue = data[idx * n] * scale_factor;
      image[image.get_height() - 1 - y][x].green =
          data[idx * n + 1] * scale_factor;
      image[image.get_height() - 1 - y][x].red =
          data[idx * n + 2] * scale_factor;
      image[image.get_height() - 1 - y][x].alpha =
          data[idx * n + 3] * scale_factor;
    }
  };
};

template <typename T, typename Components>
template <typename, typename>
Texture2D<T, Components>::Texture2D(const std::string& filepath,
                                    InterpolationMode  interp_mode_min,
                                    InterpolationMode  interp_mode_mag,
                                    WrapMode           wrap_mode_s,
                                    WrapMode           wrap_mode_t) {
  gl::create_textures(GL_TEXTURE_2D, 1, &m_id);
  assert(filepath.substr(filepath.size() - 3, 3) == "png");
  bind();
  load_png(filepath);
  set_interpolation_mode_min(interp_mode_min);
  set_interpolation_mode_mag(interp_mode_mag);
  set_wrap_mode_s(wrap_mode_s);
  set_wrap_mode_t(wrap_mode_t);
}

template <typename T, typename Components>
Texture2D<T, Components>::Texture2D(unsigned int width, unsigned int height,
                                    InterpolationMode interp_mode_min,
                                    InterpolationMode interp_mode_mag,
                                    WrapMode wrap_mode_s, WrapMode wrap_mode_t)
    : Texture(), m_width(0), m_height(0) {
  gl::create_textures(GL_TEXTURE_2D, 1, &m_id);
  bind();
  resize(width, height);
  m_is_consistent = false;
  set_interpolation_mode_min(interp_mode_min);
  set_interpolation_mode_mag(interp_mode_mag);
  set_wrap_mode_s(wrap_mode_s);
  set_wrap_mode_t(wrap_mode_t);
}

template <typename T, typename Components>
Texture2D<T, Components>::Texture2D(const Texture2D& other)
    : Texture2D(other.m_width, other.m_height) {
  gl::copy_image_sub_data(other.m_id, GL_TEXTURE_2D, 0, 0, 0, 0, this->m_id,
                          GL_TEXTURE_2D, 0, 0, 0, 0, m_width, m_height, 1);
}

template <typename T, typename Components>
Texture2D<T, Components>::Texture2D(Texture2D&& other)
    : Texture(std::move(other)),
      m_width(other.m_width),
      m_height(other.m_height) {}

template <typename T, typename Components>
Texture2D<T, Components>::Texture2D(unsigned int width, unsigned int height,
                                    const std::vector<T>& data,
                                    InterpolationMode     interp_mode_min,
                                    InterpolationMode     interp_mode_mag,
                                    WrapMode wrap_mode_s, WrapMode wrap_mode_t)
    : Texture(), m_width(width), m_height(height) {
  gl::create_textures(GL_TEXTURE_2D, 1, &m_id);
  bind();
  upload_data(data);
  set_interpolation_mode_min(interp_mode_min);
  set_interpolation_mode_mag(interp_mode_mag);
  set_wrap_mode_s(wrap_mode_s);
  set_wrap_mode_t(wrap_mode_t);
}

template <typename T, typename Components>
template <typename S>
Texture2D<T, Components>::Texture2D(unsigned int width, unsigned int height,
                                    const std::vector<S>& _data,
                                    InterpolationMode     interp_mode_min,
                                    InterpolationMode     interp_mode_mag,
                                    WrapMode wrap_mode_s, WrapMode wrap_mode_t)
    : Texture(), m_width(width), m_height(height) {
  std::vector<T> data;
  data.reserve(_data.size());
  for (const auto& date : _data) data.push_back(static_cast<T>(date));
  gl::create_textures(GL_TEXTURE_2D, 1, &m_id);
  bind();
  upload_data(data);
  set_interpolation_mode_min(interp_mode_min);
  set_interpolation_mode_mag(interp_mode_mag);
  set_wrap_mode_s(wrap_mode_s);
  set_wrap_mode_t(wrap_mode_t);
}

template <typename T, typename Components>
void Texture2D<T, Components>::bind(unsigned int i) const {
  assert(i < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
  gl::active_texture(GL_TEXTURE0 + i);
  gl::bind_texture(GL_TEXTURE_2D, m_id);
}

template <typename T, typename Components>
void Texture2D<T, Components>::unbind(unsigned int i) {
  gl::active_texture(GL_TEXTURE0 + i);
  gl::bind_texture(GL_TEXTURE_2D, 0);
}

template <typename T, typename Components>
void Texture2D<T, Components>::bind_image_texture(unsigned int i) const {
  gl::bind_image_texture(i, this->m_id, 0, GL_FALSE, 0, GL_READ_WRITE,
                         internal_format);
}

template <typename T, typename Components>
void Texture2D<T, Components>::unbind_image_texture(unsigned int i) {
  gl::bind_image_texture(i, 0, 0, GL_FALSE, 0, GL_READ_WRITE, internal_format);
}

template <typename T, typename Components>
void Texture2D<T, Components>::set_interpolation_mode(InterpolationMode mode) {
  set_interpolation_mode_min(mode);
  set_interpolation_mode_mag(mode);
}

template <typename T, typename Components>
void Texture2D<T, Components>::set_interpolation_mode_min(
    InterpolationMode mode) {
  gl::texture_parameter_i(m_id, GL_TEXTURE_MIN_FILTER, mode);
}

template <typename T, typename Components>
void Texture2D<T, Components>::set_interpolation_mode_mag(
    InterpolationMode mode) {
  gl::texture_parameter_i(m_id, GL_TEXTURE_MAG_FILTER, mode);
}

template <typename T, typename Components>
void Texture2D<T, Components>::set_wrap_mode(WrapMode mode) {
  set_wrap_mode_s(mode);
  set_wrap_mode_t(mode);
}

template <typename T, typename Components>
void Texture2D<T, Components>::set_wrap_mode_s(WrapMode mode) {
  gl::texture_parameter_i(m_id, GL_TEXTURE_WRAP_S, mode);
}

template <typename T, typename Components>
void Texture2D<T, Components>::set_wrap_mode_t(WrapMode mode) {
  gl::texture_parameter_i(m_id, GL_TEXTURE_WRAP_T, mode);
}

template <typename T, typename Components>
auto Texture2D<T, Components>::width() const {
  return m_width;
}

template <typename T, typename Components>
auto Texture2D<T, Components>::height() const {
  return m_height;
}

template <typename T, typename Components>
template <typename, typename>
void Texture2D<T, Components>::load_png(const std::string& filepath) {
  using tex_png_t = tex_png<T, Components>;
  typename tex_png_t::type image;
  image.read(filepath);
  m_width  = image.get_width();
  m_height = image.get_height();
  std::vector<T> data;
  data.reserve(m_width * m_height * n);
  for (png::uint_32 y = image.get_height(); y > 0; --y)
    for (png::uint_32 x = 0; x < image.get_width(); ++x)
      tex_png_t::load_pixel(data, image, x, y);
  if constexpr (is_float) {
    auto normalize = [](auto d) { return d / 255.0f; };
    std::transform(begin(data), end(data), begin(data), normalize);
  }

  upload_data(data);
}

template <typename T, typename Components>
template <typename, typename>
void Texture2D<T, Components>::save_png(const std::string& filepath,
                                        T                  scale_factor) {
  using tex_png_t               = tex_png<T, Components>;
  auto                     data = download_data();
  typename tex_png_t::type image(m_width, m_height);
  for (unsigned int y = 0; y < image.get_height(); ++y)
    for (png::uint_32 x = 0; x < image.get_width(); ++x) {
      unsigned int idx = x + m_width * y;
      tex_png_t::save_pixel(data, image, x, y, idx, scale_factor);
    }
  image.write(filepath);
}

template <typename T, typename Components>
void Texture2D<T, Components>::upload_data(const std::vector<T>& data) {
  gl::tex_image_2d(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0,
                   format, type, data.data());
}

template <typename T, typename Components>
void Texture2D<T, Components>::upload_data(unsigned int          width,
                                           unsigned int          height,
                                           const std::vector<T>& data) {
  m_width  = width;
  m_height = height;
  upload_data(data);
}

template <typename T, typename Components>
void Texture2D<T, Components>::set_data(const PixelUnpackBuffer<T>& pbo) {
  bind();
  pbo.bind();
  gl::tex_sub_image_2d(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, format, type,
                       0);
  pbo.unbind();
}

template <typename T, typename Components>
void Texture2D<T, Components>::resize(unsigned int width, unsigned int height) {
  if (m_width != width || m_height != height) {
    m_width  = width;
    m_height = height;
    gl::tex_image_2d(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0,
                     format, type, nullptr);
    m_is_consistent = false;
  }
}

template <typename T, typename Components>
std::vector<T> Texture2D<T, Components>::download_data() const {
  std::vector<T> data(n * m_width * m_height);
  gl::get_texture_image(m_id, 0, format, type,
                        m_width * m_height * n * sizeof(T), data.data());
  return data;
}

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif