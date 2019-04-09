#ifndef __YAVIN_TEX_PNG_H__
#define __YAVIN_TEX_PNG_H__

#include <png++/png.hpp>
#include "texcomponents.h"

//==============================================================================
namespace yavin {
//==============================================================================

template <typename type, typename tex_t>
struct tex_png;

//==============================================================================
template <typename type>
struct tex_png<type, R> {
  static constexpr size_t num_components = R::num_components;
  using png_t                            = png::image<png::rgb_pixel>;
  static constexpr auto load_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<type>(image[y - 1][x].red));
    data.push_back(static_cast<type>(image[y - 1][x].green));
    data.push_back(static_cast<type>(image[y - 1][x].blue));
  };
  static constexpr auto save_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y, size_t idx) {
    if constexpr (std::is_same_v<float, type>) {
      image[image.get_height() - 1 - y][x].red =
          data[idx * num_components] * 255.0f;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components] * 255.0f;
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components] * 255.0f;
    } else {
      image[image.get_height() - 1 - y][x].red   = data[idx * num_components];
      image[image.get_height() - 1 - y][x].green = data[idx * num_components];
      image[image.get_height() - 1 - y][x].blue  = data[idx * num_components];
    }
  };
};

//==============================================================================
template <typename type>
struct tex_png<type, RGB> {
  static constexpr size_t num_components = RGB::num_components;
  using png_t                            = png::image<png::rgb_pixel>;
  static constexpr auto load_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<type>(image[y - 1][x].red));
    data.push_back(static_cast<type>(image[y - 1][x].green));
    data.push_back(static_cast<type>(image[y - 1][x].blue));
  };
  static constexpr auto save_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y, size_t idx) {
    if constexpr (std::is_same_v<float, type>) {
      image[image.get_height() - 1 - y][x].red =
          data[idx * num_components] * 255.0f;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1] * 255.0f;
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components + 2] * 255.0f;
    } else {
      image[image.get_height() - 1 - y][x].red = data[idx * num_components];
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1];
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components + 2];
    }
  };
};

//==============================================================================
template <typename type>
struct tex_png<type, BGR> {
  static constexpr size_t num_components = BGR::num_components;
  using png_t                            = png::image<png::rgb_pixel>;
  static constexpr auto load_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<type>(image[y - 1][x].blue));
    data.push_back(static_cast<type>(image[y - 1][x].green));
    data.push_back(static_cast<type>(image[y - 1][x].red));
  };
  static constexpr auto save_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y, size_t idx) {
    if constexpr (std::is_same_v<float, type>) {
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components] * 255.0f;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1] * 255.0f;
      image[image.get_height() - 1 - y][x].red =
          data[idx * num_components + 2] * 255.0f;
    } else {
      image[image.get_height() - 1 - y][x].blue = data[idx * num_components];
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1];
      image[image.get_height() - 1 - y][x].red = data[idx * num_components + 2];
    }
  };
};

//==============================================================================
template <typename type>
struct tex_png<type, RGBA> {
  static constexpr size_t num_components = RGBA::num_components;
  using png_t                            = png::image<png::rgba_pixel>;
  static constexpr auto load_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<type>(image[y - 1][x].red));
    data.push_back(static_cast<type>(image[y - 1][x].green));
    data.push_back(static_cast<type>(image[y - 1][x].blue));
    data.push_back(static_cast<type>(image[y - 1][x].alpha));
  };
  static constexpr auto save_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y, size_t idx) {
    if constexpr (std::is_same_v<float, type>) {
      image[image.get_height() - 1 - y][x].red =
          data[idx * num_components] * 255.0f;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1] * 255.0f;
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components + 2] * 255.0f;
      image[image.get_height() - 1 - y][x].alpha =
          data[idx * num_components + 3] * 255.0f;
    } else {
      image[image.get_height() - 1 - y][x].red = data[idx * num_components];
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1];
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components + 2];
      image[image.get_height() - 1 - y][x].alpha =
          data[idx * num_components + 3];
    }
  };
};

//==============================================================================
template <typename type>
struct tex_png<type, BGRA> {
  static constexpr size_t num_components = BGRA::num_components;
  using png_t                            = png::image<png::rgba_pixel>;
  static constexpr auto load_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y) {
    data.push_back(static_cast<type>(image[y - 1][x].blue));
    data.push_back(static_cast<type>(image[y - 1][x].green));
    data.push_back(static_cast<type>(image[y - 1][x].red));
    data.push_back(static_cast<type>(image[y - 1][x].alpha));
  };
  static constexpr auto save_pixel = [](std::vector<type>& data, png_t& image,
                                        size_t x, size_t y, size_t idx) {
    if constexpr (std::is_same_v<float, type>) {
      image[image.get_height() - 1 - y][x].blue =
          data[idx * num_components] * 255.0f;
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1] * 255.0f;
      image[image.get_height() - 1 - y][x].red =
          data[idx * num_components + 2] * 255.0f;
      image[image.get_height() - 1 - y][x].alpha =
          data[idx * num_components + 3] * 255.0f;
    } else {
      image[image.get_height() - 1 - y][x].blue = data[idx * num_components];
      image[image.get_height() - 1 - y][x].green =
          data[idx * num_components + 1];
      image[image.get_height() - 1 - y][x].red = data[idx * num_components + 2];
      image[image.get_height() - 1 - y][x].alpha =
          data[idx * num_components + 3];
    }
  };
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
