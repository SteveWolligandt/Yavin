#ifndef __YAVIN_TEXTURE2D_H__
#define __YAVIN_TEXTURE2D_H__

#include <cassert>
#include <initializer_list>
#include <iostream>
#include <png++/png.hpp>
#include <string>
#include <type_traits>
#include <vector>
#include "TexHelper.h"
#include "Texture.h"

namespace Yavin {
template <typename T, typename Components>
class Texture2D : public Yavin::Texture {
 public:
  Texture2D(const std::string& filepath, bool direct_upload = true, bool keep_data_on_cpu = false);

  Texture2D(const Texture2D& other);

  Texture2D(Texture2D&& other);

  Texture2D(unsigned int width, unsigned int height);

  Texture2D(unsigned int width, unsigned int height, const std::vector<T>& data, bool direct_upload = true,
            bool keep_data_on_cpu = false);

  Texture2D(unsigned int width, unsigned int height, std::vector<T>&& data, bool direct_upload = true,
            bool keep_data_on_cpu = false);

  Texture2D(unsigned int width, unsigned int height, std::initializer_list<T>&& list, bool direct_upload = true,
            bool keep_data_on_cpu = false);

  template <typename S>
  Texture2D(unsigned int width, unsigned int height, const std::vector<S>& data, bool direct_upload = true,
            bool keep_data_on_cpu = false);

  template <typename S>
  Texture2D(unsigned int width, unsigned int height, const std::initializer_list<S>& list, bool direct_upload = true,
            bool keep_data_on_cpu = false);

  void        bind(unsigned int i = 0);
  static void unbind(unsigned int i = 0);

  void set_interpolation_mode(Yavin::Texture::InterpolationMode mode);
  void set_interpolation_mode_min(Yavin::Texture::InterpolationMode mode);
  void set_interpolation_mode_mag(Yavin::Texture::InterpolationMode mode);

  void set_wrap_mode(Yavin::Texture::WrapMode mode);
  void set_wrap_mode_s(Yavin::Texture::WrapMode mode);
  void set_wrap_mode_t(Yavin::Texture::WrapMode mode);

  auto        width() const;
  auto        height() const;
  auto        is_consistent() const;
  const auto& data() const;

  void load_png(const std::string& filepath, bool direct_upload = true, bool keep_data_on_cpu = false);

  void save_png(const std::string& filepath);

  void upload_data(unsigned int width, unsigned int height, const std::vector<T>& data, bool keep_data_on_cpu = false);

  void upload_data(unsigned int width, unsigned int height, std::vector<T>&& data, bool keep_data_on_cpu = false);

  void upload_data(unsigned int width, unsigned int height, std::initializer_list<T>&& data,
                   bool keep_data_on_cpu = false);

  void upload_data(bool keep_data_on_cpu = false);

  void        resize(unsigned int w, unsigned int h);
  const auto& download_data();

  static constexpr unsigned int n               = TexHelper::comb<T, Components>::num_components;
  static constexpr GLint        internal_format = TexHelper::comb<T, Components>::internal_format;
  static constexpr GLenum       format          = TexHelper::comb<T, Components>::format;
  static constexpr GLenum       type            = TexHelper::comb<T, Components>::type;

 protected:
  unsigned int   m_width, m_height;
  bool           m_is_consistent = false;
  std::vector<T> m_data;
};

template <typename T, typename Components>
Texture2D<T, Components>::Texture2D(const std::string& filepath, bool direct_upload, bool keep_data_on_cpu) {
  glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
  gl_error_check("glCreateTextures");
  assert(filepath.substr(filepath.size() - 3, 3) == "png");
  if (direct_upload) bind();
  load_png(filepath, direct_upload, keep_data_on_cpu);
}

template <typename T, typename Components>
Texture2D<T, Components>::Texture2D(unsigned int width, unsigned int height) : Texture(), m_width(0), m_height(0) {
  glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
  gl_error_check("glCreateTextures");
  bind();
  resize(width, height);
  m_is_consistent = false;
}

template <typename T, typename Components>
Texture2D<T, Components>::Texture2D(const Texture2D& other)
    : Texture(),
      m_width(other.m_width),
      m_height(other.m_height),
      m_is_consistent(other.m_is_consistent),
      m_data(other.m_data) {
  if (m_is_consistent) {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    gl_error_check("glCreateTextures");
    bind();
    upload_data(true);
  }
}

template <typename T, typename Components>
Texture2D<T, Components>::Texture2D(Texture2D&& other)
    : Texture(),
      m_width(other.m_width),
      m_height(other.m_height),
      m_is_consistent(other.m_is_consistent),
      m_data(std::move(other.m_data)) {
  other.m_dont_delete = false;
  if (m_is_consistent) {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    gl_error_check("glCreateTextures");
    bind();
    upload_data(true);
  }
}

template <typename T, typename Components>
Texture2D<T, Components>::Texture2D(unsigned int width, unsigned int height, const std::vector<T>& data,
                                    bool direct_upload, bool keep_data_on_cpu)
    : Texture(), m_width(width), m_height(height), m_data(data) {
  if (direct_upload) {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    gl_error_check("glCreateTextures");
    bind();
    upload_data(keep_data_on_cpu);
  }
}

template <typename T, typename Components>
Texture2D<T, Components>::Texture2D(unsigned int width, unsigned int height, std::vector<T>&& data, bool direct_upload,
                                    bool keep_data_on_cpu)
    : Texture(), m_width(width), m_height(height), m_data(std::move(data)) {
  if (direct_upload) {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    gl_error_check("glCreateTextures");
    bind();
    upload_data(keep_data_on_cpu);
  }
}

template <typename T, typename Components>
Texture2D<T, Components>::Texture2D(unsigned int width, unsigned int height, std::initializer_list<T>&& list,
                                    bool direct_upload, bool keep_data_on_cpu)
    : Texture(), m_width(width), m_height(height), m_data(std::move(list)) {
  if (direct_upload) {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    gl_error_check("glCreateTextures");
    bind();
    upload_data(keep_data_on_cpu);
  }
}

template <typename T, typename Components>
template <typename S>
Texture2D<T, Components>::Texture2D(unsigned int width, unsigned int height, const std::vector<S>& data,
                                    bool direct_upload, bool keep_data_on_cpu)
    : Texture(), m_width(width), m_height(height) {
  m_data.reserve(data.size());
  for (const auto& date : data) m_data.push_back(static_cast<T>(date));

  if (direct_upload) {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    gl_error_check("glCreateTextures");
    bind();
    upload_data(keep_data_on_cpu);
  }
}

template <typename T, typename Components>
template <typename S>
Texture2D<T, Components>::Texture2D(unsigned int width, unsigned int height, const std::initializer_list<S>& list,
                                    bool direct_upload, bool keep_data_on_cpu)
    : Texture(), m_width(width), m_height(height) {
  for (const auto& date : list) m_data.push_back(static_cast<T>(date));
  if (direct_upload) {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    gl_error_check("glCreateTextures");
    bind();
    upload_data(keep_data_on_cpu);
  }
}

template <typename T, typename Components>
void Texture2D<T, Components>::bind(unsigned int i) {
  assert(i < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
  glActiveTexture(GL_TEXTURE0 + i);
  gl_error_check("glActiveTexture");
  glBindTexture(GL_TEXTURE_2D, m_id);
  gl_error_check("glBindTexture");
}

template <typename T, typename Components>
void Texture2D<T, Components>::unbind(unsigned int i) {
  glActiveTexture(GL_TEXTURE0 + i);
  gl_error_check("glActiveTexture");
  glBindTexture(GL_TEXTURE_2D, 0);
  gl_error_check("glBindTexture");
}

template <typename T, typename Components>
void Texture2D<T, Components>::set_interpolation_mode(InterpolationMode mode) {
  set_interpolation_mode_min(mode);
  set_interpolation_mode_mag(mode);
}

template <typename T, typename Components>
void Texture2D<T, Components>::set_interpolation_mode_min(InterpolationMode mode) {
  glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, mode);
  gl_error_check("glTextureParameteri - GL_TEXTURE_MIN_FILTER");
}

template <typename T, typename Components>
void Texture2D<T, Components>::set_interpolation_mode_mag(InterpolationMode mode) {
  glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, mode);
  gl_error_check("glTextureParameteri - GL_TEXTURE_MAG_FILTER");
}

template <typename T, typename Components>
void Texture2D<T, Components>::set_wrap_mode(WrapMode mode) {
  set_wrap_mode_s(mode);
  set_wrap_mode_t(mode);
}

template <typename T, typename Components>
void Texture2D<T, Components>::set_wrap_mode_s(WrapMode mode) {
  glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, mode);
  gl_error_check("glTextureParameteri - GL_TEXTURE_WRAP_S");
}

template <typename T, typename Components>
void Texture2D<T, Components>::set_wrap_mode_t(WrapMode mode) {
  glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, mode);
  gl_error_check("glTextureParameteri - GL_TEXTURE_WRAP_T");
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
auto Texture2D<T, Components>::is_consistent() const {
  return m_is_consistent;
}

template <typename T, typename Components>
const auto& Texture2D<T, Components>::data() const {
  return m_data;
}

template <typename T, typename Components>
void Texture2D<T, Components>::load_png(const std::string& filepath, bool direct_upload, bool keep_data_on_cpu) {
  if constexpr (std::is_same<Components, RGB>::value) {
    png::image<png::rgb_pixel> image;
    image.read(filepath);
    m_width  = image.get_width();
    m_height = image.get_height();
    m_data.reserve(m_width * m_height * n);
    for (png::uint_32 y = image.get_height(); y > 0; --y)
      for (png::uint_32 x = 0; x < image.get_width(); ++x) {
        m_data.push_back(static_cast<T>(image[y - 1][x].red));
        m_data.push_back(static_cast<T>(image[y - 1][x].green));
        m_data.push_back(static_cast<T>(image[y - 1][x].blue));
      }
    if constexpr (std::is_same<float, T>::value)
      for (auto& date : m_data) date /= 255.0f;
  } else if constexpr (std::is_same<Components, RGBA>::value) {
    png::image<png::rgba_pixel> image;
    image.read(filepath);
    m_width  = image.get_width();
    m_height = image.get_height();
    m_data.reserve(m_width * m_height * n);
    for (png::uint_32 y = image.get_height(); y > 0; --y)
      for (png::uint_32 x = 0; x < image.get_width(); ++x) {
        m_data.push_back(static_cast<T>(image[y - 1][x].red));
        m_data.push_back(static_cast<T>(image[y - 1][x].green));
        m_data.push_back(static_cast<T>(image[y - 1][x].blue));
        m_data.push_back(static_cast<T>(image[y - 1][x].alpha));
      }
    if constexpr (std::is_same<float, T>::value)
      for (auto& date : m_data) date /= 255.0f;
  } else if constexpr (std::is_same<Components, BGR>::value) {
    png::image<png::rgb_pixel> image;
    image.read(filepath);
    m_width  = image.get_width();
    m_height = image.get_height();
    m_data.reserve(m_width * m_height * n);
    for (png::uint_32 y = image.get_height(); y > 0; --y)
      for (png::uint_32 x = 0; x < image.get_width(); ++x) {
        m_data.push_back(static_cast<T>(image[y - 1][x].blue));
        m_data.push_back(static_cast<T>(image[y - 1][x].green));
        m_data.push_back(static_cast<T>(image[y - 1][x].red));
      }
    if constexpr (std::is_same<float, T>::value)
      for (auto& date : m_data) date /= 255.0f;
  } else if constexpr (std::is_same<Components, BGRA>::value) {
    png::image<png::rgba_pixel> image;
    image.read(filepath);
    m_width  = image.get_width();
    m_height = image.get_height();
    m_data.reserve(m_width * m_height * n);
    for (png::uint_32 y = image.get_height(); y > 0; --y)
      for (png::uint_32 x = 0; x < image.get_width(); ++x) {
        m_data.push_back(static_cast<T>(image[y - 1][x].blue));
        m_data.push_back(static_cast<T>(image[y - 1][x].green));
        m_data.push_back(static_cast<T>(image[y - 1][x].red));
        m_data.push_back(static_cast<T>(image[y - 1][x].alpha));
      }
    if constexpr (std::is_same<float, T>::value)
      for (auto& date : m_data) date /= 255.0f;
  }

  if (direct_upload) upload_data(keep_data_on_cpu);
}

template <typename T, typename Components>
void Texture2D<T, Components>::save_png(const std::string& filepath) {
  if constexpr (std::is_same<Components, RGB>::value) {
    png::image<png::rgb_pixel> image(m_width, m_height);
    for (unsigned int y = 0; y < image.get_height(); ++y)
      for (png::uint_32 x = 0; x < image.get_width(); ++x) {
        unsigned int idx                           = x + m_width * y;
        image[image.get_height() - 1 - y][x].red   = m_data[idx * n];
        image[image.get_height() - 1 - y][x].green = m_data[idx * n + 1];
        image[image.get_height() - 1 - y][x].blue  = m_data[idx * n + 2];
        if constexpr (std::is_same<float, T>::value) {
          image[image.get_height() - 1 - y][x].red *= 255.0f;
          image[image.get_height() - 1 - y][x].green *= 255.0f;
          image[image.get_height() - 1 - y][x].blue *= 255.0f;
        }
      }
    image.write(filepath);
  } else if constexpr (std::is_same<Components, RGBA>::value) {
    png::image<png::rgba_pixel> image(m_width, m_height);
    for (unsigned int y = 0; y < image.get_height(); ++y)
      for (unsigned int x = 0; x < image.get_width(); ++x) {
        unsigned int idx                           = x + m_width * y;
        image[image.get_height() - 1 - y][x].red   = m_data[idx * n];
        image[image.get_height() - 1 - y][x].green = m_data[idx * n + 1];
        image[image.get_height() - 1 - y][x].blue  = m_data[idx * n + 2];
        image[image.get_height() - 1 - y][x].alpha = m_data[idx * n + 3];
        if constexpr (std::is_same<float, T>::value) {
          image[image.get_height() - 1 - y][x].red *= 255.0f;
          image[image.get_height() - 1 - y][x].green *= 255.0f;
          image[image.get_height() - 1 - y][x].blue *= 255.0f;
          image[image.get_height() - 1 - y][x].alpha *= 255.0f;
        }
      }
    image.write(filepath);
  } else if constexpr (std::is_same<Components, BGR>::value) {
    png::image<png::rgba_pixel> image(m_width, m_height);
    for (unsigned int y = 0; y < image.get_height(); ++y)
      for (unsigned int x = 0; x < image.get_width(); ++x) {
        unsigned int idx                           = x + m_width * y;
        image[image.get_height() - 1 - y][x].blue  = m_data[idx * n];
        image[image.get_height() - 1 - y][x].green = m_data[idx * n + 1];
        image[image.get_height() - 1 - y][x].red   = m_data[idx * n + 2];
        if constexpr (std::is_same<float, T>::value) {
          image[image.get_height() - 1 - y][x].blue *= 255.0f;
          image[image.get_height() - 1 - y][x].green *= 255.0f;
          image[image.get_height() - 1 - y][x].red *= 255.0f;
        }
      }
    image.write(filepath);
  } else if constexpr (std::is_same<Components, BGRA>::value) {
    png::image<png::rgba_pixel> image(m_width, m_height);
    for (unsigned int y = 0; y < image.get_height(); ++y)
      for (unsigned int x = 0; x < image.get_width(); ++x) {
        unsigned int idx                           = x + m_width * y;
        image[image.get_height() - 1 - y][x].blue  = m_data[idx * n];
        image[image.get_height() - 1 - y][x].green = m_data[idx * n + 1];
        image[image.get_height() - 1 - y][x].red   = m_data[idx * n + 2];
        image[image.get_height() - 1 - y][x].alpha = m_data[idx * n + 3];
        if constexpr (std::is_same<float, T>::value) {
          image[image.get_height() - 1 - y][x].blue *= 255.0f;
          image[image.get_height() - 1 - y][x].green *= 255.0f;
          image[image.get_height() - 1 - y][x].red *= 255.0f;
          image[image.get_height() - 1 - y][x].alpha *= 255.0f;
        }
      }
    image.write(filepath);
  }
}

template <typename T, typename Components>
void Texture2D<T, Components>::upload_data(unsigned int width, unsigned int height, const std::vector<T>& data,
                                           bool keep_data_on_cpu) {
  m_width         = width;
  m_height        = height;
  m_data          = data;
  m_is_consistent = false;
  upload_data(keep_data_on_cpu);
}

template <typename T, typename Components>
void Texture2D<T, Components>::upload_data(unsigned int width, unsigned int height, std::vector<T>&& data,
                                           bool keep_data_on_cpu) {
  m_width         = width;
  m_height        = height;
  m_data          = std::move(data);
  m_is_consistent = false;
  upload_data(keep_data_on_cpu);
}

template <typename T, typename Components>
void Texture2D<T, Components>::upload_data(unsigned int width, unsigned int height, std::initializer_list<T>&& list,
                                           bool keep_data_on_cpu) {
  m_width         = width;
  m_height        = height;
  m_data          = std::move(list);
  m_is_consistent = false;
  upload_data(keep_data_on_cpu);
}

template <typename T, typename Components>
void Texture2D<T, Components>::upload_data(bool keep_data_on_cpu) {
  if (!m_is_consistent) {
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, format, type, &m_data[0]);
    gl_error_check("glTexImage2D");
    m_is_consistent = true;
  }
  if (!keep_data_on_cpu) {
    m_data.clear();
    m_is_consistent = false;
  }
}

template <typename T, typename Components>
void Texture2D<T, Components>::resize(unsigned int width, unsigned int height) {
  if (m_width != width || m_height != height) {
    m_width  = width;
    m_height = height;
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, format, type, nullptr);
    gl_error_check("glTexImage2D");
    m_is_consistent = false;
  }
}

template <typename T, typename Components>
const auto& Texture2D<T, Components>::download_data() {
  m_data.resize(n * m_width * m_height);
  glGetTextureImage(m_id, 0, format, type, m_width * m_height * n * sizeof(T), &m_data[0]);
  gl_error_check("glGetTextureImage");
  m_is_consistent = true;
  return m_data;
}
}  // namespace Yavin

#endif