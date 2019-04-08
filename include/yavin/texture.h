#ifndef __YAVIN_TEXTURE_H__
#define __YAVIN_TEXTURE_H__

#include <yavin/settings.h>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <iostream>
#include <utility>
#include "error_check.h"
#include "gl_functions.h"
#include "id_holder.h"
#include "pixel_unpack_buffer.h"
#include "tex_components.h"
#include "tex_settings.h"
#include "tex_target.h"
#include "tex_png.h"
#include "type.h"

#ifdef USE_PNG
#include "tex_png.h"
#endif

//==============================================================================
namespace yavin {
//==============================================================================

enum WrapMode {
  CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
  CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
  REPEAT          = GL_REPEAT,
  MIRRORED_REPEAT = GL_MIRRORED_REPEAT
};

//==============================================================================
enum InterpolationMode {
  NEAREST                = GL_NEAREST,
  LINEAR                 = GL_LINEAR,
  NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
  LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
  NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
  LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
};

//==============================================================================
enum CompareFunc {
  NEVER    = GL_NEVER,
  LESS     = GL_LESS,
  LEQUAL   = GL_LEQUAL,
  GREATER  = GL_GREATER,
  NOTEQUAL = GL_NOTEQUAL,
  GEQUAL   = GL_GEQUAL,
  ALWAYS   = GL_ALWAYS
};

//==============================================================================
enum CompareMode {
  COMPARE_R_TO_TEXTURE = GL_COMPARE_R_TO_TEXTURE,
  NONE                 = GL_NONE
};

//==============================================================================
template <unsigned int D, typename T, typename C>
class texture : public id_holder<GLuint> {
  static_assert(D >= 1 && D <= 3,
                "number of dimensions must be between 1 and 3");

 public:
  //============================================================================
  using type                                  = T;
  using components                            = C;
  static constexpr auto target                = tex::target_v<D>;
  static constexpr auto default_interpolation = LINEAR;
  static constexpr auto default_wrap_mode     = REPEAT;
  static constexpr auto num_components        = components::num_components;
  static constexpr auto num_dimensions        = D;

  static constexpr auto gl_internal_format =
      tex::settings<T, C>::internal_format;
  static constexpr auto gl_format = tex::settings<T, C>::format;
  static constexpr auto gl_type   = tex::settings<T, C>::type;
  static constexpr std::array<GLenum, 3> wrapmode_indices{
      GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R};

  static constexpr bool is_readable =
      (D == 2 && std::is_same_v<C, R>) || (D == 2 && std::is_same_v<C, RGB>) ||
      (D == 2 && std::is_same_v<C, RGBA>) ||
      (D == 2 && std::is_same_v<C, BGR>) || (D == 2 && std::is_same_v<C, BGRA>);

 protected:
  //============================================================================
  std::array<size_t, D> m_size;

 public:
  //============================================================================
  texture() : texture{std::make_index_sequence<D>{}} {}
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  template <size_t... Is>
  texture(std::index_sequence<Is...>) : m_size{((void)Is, 0)...} {
    create_id();
    set_wrap_mode(default_wrap_mode);
    set_interpolation_mode(default_interpolation);
  }

  //----------------------------------------------------------------------------
  //! TODO: copy wrap and interpolation modes
  texture(const texture& other) : texture{} { copy_data(other); }

  //----------------------------------------------------------------------------
  texture(texture&& other)
      : id_holder{std::move(other)}, m_size{std::move(other.m_size)} {}

  //----------------------------------------------------------------------------
  auto& operator=(const texture& other) {
    copy_data(other);
    return *this;
  }

  //----------------------------------------------------------------------------
  auto& operator=(texture&& other) {
    id_holder::operator=(std::move(other));
    m_size             = std::move(other.m_size);
    return *this;
  }

  //----------------------------------------------------------------------------
  ~texture() {
    if (id()) { gl::delete_textures(1, &id()); }
  }

  //----------------------------------------------------------------------------
  template <typename... Sizes>
  texture(Sizes... sizes) : m_size{static_cast<size_t>(sizes)...} {
    static_assert(sizeof...(Sizes) == D,
                  "number of sizes does not match number of dimensions");
    static_assert((std::is_integral_v<Sizes> && ...),
                  "types of sizes must be integral types");
    create_id();
    set_wrap_mode(default_wrap_mode);
    set_interpolation_mode(default_interpolation);
    resize(sizes...);
  }

  //----------------------------------------------------------------------------
  template <typename S, typename... Sizes>
  texture(const std::vector<S>& data, Sizes... sizes) : m_size{sizes...} {
    static_assert(sizeof...(Sizes) == D,
                  "number of sizes does not match number of dimensions");
    static_assert((std::is_integral_v<Sizes> && ...),
                  "types of sizes must be integral types");
    create_id();
    set_interpolation_mode(default_interpolation);
    set_wrap_mode(default_wrap_mode);
    upload_data(data);
  }

  //----------------------------------------------------------------------------
  template <size_t... Is>
  texture(InterpolationMode interp_mode, WrapMode wrap_mode,
          std::index_sequence<Is...>)
      : m_size{((void)Is, 0)...} {
    create_id();
    set_interpolation_mode(interp_mode);
    set_wrap_mode(wrap_mode);
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  texture(InterpolationMode interp_mode, WrapMode wrap_mode)
      : texture{interp_mode, wrap_mode, std::make_index_sequence<D>{}} {}

  //----------------------------------------------------------------------------
  template <typename... Sizes>
  texture(InterpolationMode interp_mode, WrapMode wrap_mode, Sizes... sizes)
      : m_size{sizes...} {
    static_assert(sizeof...(Sizes) == D,
                  "number of sizes does not match number of dimensions");
    static_assert((std::is_integral_v<Sizes> && ...),
                  "types of sizes must be integral types");
    create_id();
    set_interpolation_mode(interp_mode);
    set_wrap_mode(wrap_mode);
    resize(sizes...);
  }

  //----------------------------------------------------------------------------
  template <typename S, typename... Sizes>
  texture(InterpolationMode interp_mode, WrapMode wrap_mode,
          const std::vector<S>& data, Sizes... sizes)
      : m_size{sizes...} {
    static_assert(sizeof...(Sizes) == D,
                  "number of sizes does not match number of dimensions");
    static_assert((std::is_integral_v<Sizes> && ...),
                  "types of sizes must be integral types");
    create_id();
    set_interpolation_mode(interp_mode);
    set_wrap_mode(wrap_mode);
    upload_data(data);
  }

  //----------------------------------------------------------------------------
  texture(const std::string& filepath) : texture{} { read(filepath); }

  //----------------------------------------------------------------------------
  texture(InterpolationMode interp_mode, WrapMode wrap_mode,
          const std::string& filepath)
      : texture{interp_mode, wrap_mode} {
    read(filepath);
  }

 private:
  //----------------------------------------------------------------------------
  void create_id() { gl::create_textures(target, 1, &id()); }

 public:
  //----------------------------------------------------------------------------
  void bind(GLuint unit = 0) const {
    assert(unit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    gl::active_texture(GL_TEXTURE0 + unit);
    gl::bind_texture(target, id());
  }

  //----------------------------------------------------------------------------
  static void unbind(GLuint unit = 0) {
    gl::active_texture(GL_TEXTURE0 + unit);
    gl::bind_texture(target, 0);
  }

  //------------------------------------------------------------------------------
  void bind_image_texture(GLuint unit) const {
    gl::bind_image_texture(unit, id(), 0, GL_FALSE, 0, GL_READ_ONLY,
                           gl_internal_format);
  }

  //------------------------------------------------------------------------------
  void bind_image_texture(GLuint unit) {
    gl::bind_image_texture(unit, id(), 0, GL_FALSE, 0, GL_READ_WRITE,
                           gl_internal_format);
  }

  //------------------------------------------------------------------------------
  void bind_image_texture_read_write(GLuint unit) { bind_image_texture(unit); }

  //------------------------------------------------------------------------------
  void bind_image_texture_write(GLuint unit) {
    gl::bind_image_texture(unit, id(), 0, GL_FALSE, 0, GL_WRITE_ONLY,
                           gl_internal_format);
  }

  //------------------------------------------------------------------------------
  void unbind_image_texture(GLuint unit) {
    gl::bind_image_texture(unit, 0, 0, GL_FALSE, 0, GL_READ_WRITE,
                           gl_internal_format);
  }

  //------------------------------------------------------------------------------
  template <unsigned int D_ = D, typename = std::enable_if_t<D_ == 3>>
  void bind_image_texture_layer(GLuint unit, GLint layer) const {
    gl::bind_image_texture(unit, id(), 0, GL_TRUE, layer, GL_READ_ONLY,
                           gl_internal_format);
  }

  //------------------------------------------------------------------------------
  template <unsigned int D_ = D, typename = std::enable_if_t<D_ == 3>>
  void bind_image_texture_layer(GLuint unit, GLint layer) {
    gl::bind_image_texture(unit, id(), 0, GL_TRUE, layer, GL_READ_WRITE,
                           gl_internal_format);
  }

  //------------------------------------------------------------------------------
  template <unsigned int D_ = D, typename = std::enable_if_t<D_ == 3>>
  static void unbind_image_texture_layer(GLuint unit, GLint layer) {
    gl::bind_image_texture(unit, 0, 0, GL_TRUE, layer, GL_READ_WRITE,
                           gl_internal_format);
  }

  //----------------------------------------------------------------------------
  template <size_t... Is>
  size_t num_texels(std::index_sequence<Is...>) const {
    return (m_size[Is] * ...);
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  size_t num_texels() const {
    return num_texels(std::make_index_sequence<D>{});
  }

  //----------------------------------------------------------------------------
  void copy_data(const texture& other) {
    m_size = other.m_size;
    if constexpr (D == 1)
      gl::copy_image_sub_data(other.id(), target, 0, 0, 0, 0, id(),
                              GL_TEXTURE_1D, 0, 0, 0, 0, m_size[0], 1, 1);
    else if (D == 2)
      gl::copy_image_sub_data(other.id(), target, 0, 0, 0, 0, id(),
                              GL_TEXTURE_1D, 0, 0, 0, 0, m_size[0], m_size[1],
                              1);

    else
      gl::copy_image_sub_data(other.id(), target, 0, 0, 0, 0, id(),
                              GL_TEXTURE_1D, 0, 0, 0, 0, m_size[0], m_size[1],
                              m_size[2]);
  }

  //------------------------------------------------------------------------------
  template <typename... Sizes>
  void resize(Sizes... sizes) {
    static_assert(sizeof...(Sizes) == D);
    static_assert((std::is_integral_v<Sizes> && ...));
    bind();
    m_size = std::array<size_t, D>{static_cast<size_t>(sizes)...};
    if constexpr (D == 1)
      gl::tex_image_1d(target, 0, gl_internal_format, width(), 0, gl_format,
                       gl_type, nullptr);
    else if constexpr (D == 2)
      gl::tex_image_2d(target, 0, gl_internal_format, width(), height(), 0,
                       gl_format, gl_type, nullptr);
    else if constexpr (D == 3)
      gl::tex_image_3d(target, 0, gl_internal_format, width(), height(),
                       depth(), 0, gl_format, gl_type, nullptr);
  }

 private:
  //------------------------------------------------------------------------------
  template <typename S>
  void upload_data(const std::vector<S>& data) {
    std::vector<type> casted_data(data.size());
    boost::copy(data, begin(casted_data));
    upload_data(casted_data);
  }
  //------------------------------------------------------------------------------
  void upload_data(const std::vector<type>& data) {
    assert(data.size() == num_texels() * num_components);
    bind();
    if constexpr (D == 1)
      gl::tex_image_1d(target, 0, gl_internal_format, width(), 0, gl_format,
                       gl_type, data.data());
    else if constexpr (D == 2)
      gl::tex_image_2d(target, 0, gl_internal_format, width(), height(), 0,
                       gl_format, gl_type, data.data());
    else if constexpr (D == 3)
      gl::tex_image_3d(target, 0, gl_internal_format, width(), height(),
                       depth(), 0, gl_format, gl_type, data.data());
  }

 public:
  //------------------------------------------------------------------------------
  template <typename S, typename... Sizes>
  void upload_data(const std::vector<S>& data, Sizes... sizes) {
    static_assert(sizeof...(Sizes) == D);
    static_assert((std::is_integral_v<Sizes> && ...));
    m_size = std::array<size_t, D>{static_cast<size_t>(sizes)...};
    upload_data(data);
  }

  //------------------------------------------------------------------------------
  auto download_data() const {
    std::vector<type> data(num_components * num_texels());
    gl::get_texture_image(id(), 0, gl_format, gl_type,
                          num_texels() * num_components * sizeof(type),
                          data.data());
    return data;
  }

  //----------------------------------------------------------------------------
  auto width() const { return m_size[0]; }

  //----------------------------------------------------------------------------
  template <unsigned int D_ = D, typename = std::enable_if_t<(D_ > 1)>>
  auto height() const {
    return m_size[1];
  }

  //----------------------------------------------------------------------------
  template <unsigned int D_ = D, typename = std::enable_if_t<(D_ > 2)>>
  auto depth() const {
    return m_size[2];
  }

  //----------------------------------------------------------------------------
  //! setting all wrapmodes to same mode
  template <unsigned int D_ = D, typename = std::enable_if_t<(D_ > 1)>>
  void set_wrap_mode(WrapMode mode) {
    set_wrap_mode_s(mode);
    if constexpr (D > 1) set_wrap_mode_t(mode);
    if constexpr (D > 2) set_wrap_mode_r(mode);
  }

  //----------------------------------------------------------------------------
  //! setting all wrapmodes individual modes
  template <
      size_t... Is, typename... Modes,
      typename = std::enable_if_t<(std::is_same_v<Modes, WrapMode> && ...)>>
  void set_wrap_mode(std::index_sequence<Is...>, Modes... modes) {
    static_assert(sizeof...(Modes) == D);
    static_assert((std::is_same_v<Modes, WrapMode> && ...));
    using discard = int[];
    (void)discard{((void)set_wrap_mode(Is, modes), 0)...};
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //! setting all wrapmodes individual modes
  template <typename... Modes, typename = std::enable_if_t<
                                   (std::is_same_v<Modes, WrapMode> && ...)>>
  void set_wrap_mode(Modes... modes) {
    static_assert(sizeof...(Modes) == D);
    set_wrap_mode(std::make_index_sequence<D>{}, modes...);
  }

  //----------------------------------------------------------------------------
  //! setting wrapmode with index i to modes
  void set_wrap_mode(size_t i, WrapMode mode) {
    gl::texture_parameter_i(id(), wrapmode_indices[i], mode);
  }

  //----------------------------------------------------------------------------
  void set_wrap_mode_s(WrapMode mode) { set_wrap_mode(0, mode); }

  //----------------------------------------------------------------------------
  template <unsigned int D_ = D, typename = std::enable_if_t<(D_ > 1)>>
  void set_wrap_mode_t(WrapMode mode) {
    set_wrap_mode(1, mode);
  }

  //----------------------------------------------------------------------------
  template <unsigned int D_ = D, typename = std::enable_if_t<(D_ > 2)>>
  void set_wrap_mode_r(WrapMode mode) {
    set_wrap_mode(2, mode);
  }

  //------------------------------------------------------------------------------
  void set_interpolation_mode(InterpolationMode mode) {
    set_interpolation_mode_min(mode);
    set_interpolation_mode_mag(mode);
  }

  //------------------------------------------------------------------------------
  void set_interpolation_mode_min(InterpolationMode mode) {
    gl::texture_parameter_i(id(), GL_TEXTURE_MIN_FILTER, mode);
  }

  //------------------------------------------------------------------------------
  void set_interpolation_mode_mag(InterpolationMode mode) {
    gl::texture_parameter_i(id(), GL_TEXTURE_MAG_FILTER, mode);
  }

  //----------------------------------------------------------------------------
  template <typename _Components = components,
            typename = std::enable_if<std::is_same_v<_Components, Depth>>>
  void set_compare_func(CompareFunc f) {
    gl::texture_parameter_i(id(), GL_TEXTURE_COMPARE_FUNC, f);
  }

  //----------------------------------------------------------------------------
  template <typename _Components = components,
            typename = std::enable_if<std::is_same_v<_Components, Depth>>>
  void set_compare_mode(CompareMode m) {
    gl::texture_parameter_i(id(), GL_TEXTURE_COMPARE_MODE, m);
  }

  //----------------------------------------------------------------------------
  template <typename... Components>
  void clear(Components... components) {
    static_assert(sizeof...(Components) == num_components);
    static_assert((std::is_arithmetic_v<Components> && ...));
    clear({static_cast<type>(components)...});
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  template <typename type, typename components>
  void clear(const std::array<type, num_components>& col) {
    gl::clear_tex_image(id(), 0, gl_format, gl_type, col.data());
  }

  //------------------------------------------------------------------------------
  template <unsigned int D_ = D, typename = std::enable_if_t<D_ == 2>>
  void set_data(const PixelUnpackBuffer<type>& pbo) {
    bind();
    pbo.bind();
    gl::tex_sub_image_2d(GL_TEXTURE_2D, 0, 0, 0, width(), height(), gl_format,
                         gl_type, 0);
  }

  //------------------------------------------------------------------------------
  void read(const std::string filepath) {
    auto ext = filepath.substr(filepath.find_last_of(".") + 1);
    if constexpr (D == 2 && is_readable) {
      if constexpr (settings::use_png)
        if (ext == "png") {
          read_png(filepath);
          return;
        }
    }

    throw std::runtime_error("could not read fileformat ." + ext);
  }

  //------------------------------------------------------------------------------
  void write(const std::string filepath) const {
    auto ext = filepath.substr(filepath.find_last_of(".") + 1);
    if constexpr (D == 2 && is_readable) {
      if constexpr (settings::use_png)
        if (ext == "png") {
          write_png(filepath);
          return;
        }
    }
    throw std::runtime_error("could not write fileformat ." + ext);
  }

  //------------------------------------------------------------------------------
  template <unsigned int D_ = D,
            typename        = std::enable_if_t<D_ == 2 && is_readable>,
            typename        = std::enable_if_t<settings::use_png>>
  void read_png(const std::string& filepath) {
    if constexpr (settings::use_png) {
      using tex_png_t = tex_png<type, components>;
      typename tex_png_t::png_t image;
      image.read(filepath);
      m_size[0] = image.get_width();
      m_size[1] = image.get_height();
      std::vector<type> data;
      data.reserve(num_texels() * num_components);
      for (png::uint_32 y = image.get_height(); y > 0; --y)
        for (png::uint_32 x = 0; x < image.get_width(); ++x)
          tex_png_t::load_pixel(data, image, x, y);
      if constexpr (std::is_same_v<type, float>) {
        auto normalize = [](auto d) { return d / 255.0f; };
        boost::transform(data, begin(data), normalize);
      }

      upload_data(data);
    }
  }

  //------------------------------------------------------------------------------
  template <unsigned int D_ = D,
            typename        = std::enable_if_t<D_ == 2 && is_readable>,
            typename        = std::enable_if_t<settings::use_png>>
  void write_png(const std::string& filepath) const {
    if constexpr (settings::use_png) {
      using tex_png_t = tex_png<type, components>;
      typename tex_png_t::png_t image(width(), height());
      auto                      data = download_data();

      for (unsigned int y = 0; y < image.get_height(); ++y)
        for (png::uint_32 x = 0; x < image.get_width(); ++x) {
          unsigned int idx = x + width() * y;
          tex_png_t::save_pixel(data, image, x, y, idx);
        }
      image.write(filepath);
    }
  }
};

//==============================================================================
template <typename T, typename C>
using tex1D = texture<1, T, C>;

template <typename T, typename C>
using tex2D = texture<2, T, C>;

template <typename T, typename C>
using tex3D = texture<3, T, C>;

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
