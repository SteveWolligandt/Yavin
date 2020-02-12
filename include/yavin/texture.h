#ifndef YAVIN_TEXTURE_H
#define YAVIN_TEXTURE_H

#include <yavin/errorcheck.h>
#include <yavin/glfunctions.h>
#include <yavin/idholder.h>
#include <yavin/pixelunpackbuffer.h>
#include <yavin/texcomponents.h>
#include <yavin/texpng.h>
#include <yavin/texsettings.h>
#include <yavin/textarget.h>
#include <yavin/type.h>

#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <iostream>
#include <type_traits>
#include <utility>

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
  static constexpr bool is_writable =
      (D == 2 && std::is_same_v<C, R>) || (D == 2 && std::is_same_v<C, RG>) ||
      (D == 2 && std::is_same_v<C, RGB>) ||
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
  template <typename... Sizes,
            typename = std::enable_if_t<sizeof...(Sizes) == D>,
            typename = std::enable_if_t<
                (std::is_integral_v<typename std::decay_t<Sizes>> && ...)>>
  texture(Sizes... sizes) : m_size{static_cast<size_t>(sizes)...} {
    create_id();
    set_wrap_mode(default_wrap_mode);
    set_interpolation_mode(default_interpolation);
    resize(sizes...);
  }
  //----------------------------------------------------------------------------
  template <typename S, typename... Sizes,
            typename = std::enable_if_t<sizeof...(Sizes) == D>,
            typename = std::enable_if_t<
                (std::is_integral_v<typename std::decay_t<Sizes>> && ...)>>
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
  template <typename... Sizes,
            typename = std::enable_if_t<sizeof...(Sizes) == D>,
            typename = std::enable_if_t<
                (std::is_integral_v<typename std::decay_t<Sizes>> && ...)>>
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
  template <typename S, typename... Sizes,
            typename = std::enable_if_t<sizeof...(Sizes) == D>,
            typename = std::enable_if_t<
                (std::is_integral_v<typename std::decay_t<Sizes>> && ...)>>
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
    if constexpr (D == 1) {
      gl::copy_image_sub_data(other.id(), target, 0, 0, 0, 0, id(),
                              GL_TEXTURE_1D, 0, 0, 0, 0, m_size[0], 1, 1);
    } else if (D == 2) {
      gl::copy_image_sub_data(other.id(), target, 0, 0, 0, 0, id(),
                              GL_TEXTURE_1D, 0, 0, 0, 0, m_size[0], m_size[1],
                              1);

    } else {
      gl::copy_image_sub_data(other.id(), target, 0, 0, 0, 0, id(),
                              GL_TEXTURE_1D, 0, 0, 0, 0, m_size[0], m_size[1],
                              m_size[2]);
    }
  }

  //------------------------------------------------------------------------------
  template <typename... Sizes>
  void resize(Sizes... sizes) {
    static_assert(sizeof...(Sizes) == D);
    static_assert((std::is_integral_v<Sizes> && ...));
    bind();
    m_size = std::array<size_t, D>{static_cast<size_t>(sizes)...};
    if constexpr (D == 1) {
      gl::tex_image_1d(target, 0, gl_internal_format, width(), 0, gl_format,
                       gl_type, nullptr);
    } else if constexpr (D == 2) {
      gl::tex_image_2d(target, 0, gl_internal_format, width(), height(), 0,
                       gl_format, gl_type, nullptr);
    } else if constexpr (D == 3) {
      gl::tex_image_3d(target, 0, gl_internal_format, width(), height(),
                       depth(), 0, gl_format, gl_type, nullptr);
    }
  }

 private:
  //------------------------------------------------------------------------------
  template <typename S>
  void upload_data(const std::vector<S>& data) {
    upload_data(std::vector<type>(begin(data), end(data)));
  }
  //------------------------------------------------------------------------------
  void upload_data(const std::vector<type>& data) {
    assert(data.size() == num_texels() * num_components);
    bind();
    if constexpr (D == 1) {
      gl::tex_image_1d(target, 0, gl_internal_format, width(), 0, gl_format,
                       gl_type, data.data());
    } else if constexpr (D == 2) {
      gl::tex_image_2d(target, 0, gl_internal_format, width(), height(), 0,
                       gl_format, gl_type, data.data());
    } else if constexpr (D == 3) {
      gl::tex_image_3d(target, 0, gl_internal_format, width(), height(),
                       depth(), 0, gl_format, gl_type, data.data());
    }
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
  //------------------------------------------------------------------------------
  void download_data(std::vector<type>& data) const {
    assert(data.size() == num_components * num_texels());
    gl::get_texture_image(id(), 0, gl_format, gl_type,
                          num_texels() * num_components * sizeof(type),
                          data.data());
  }
  //------------------------------------------------------------------------------
  void download_data(type* data) const {
    gl::get_texture_image(id(), 0, gl_format, gl_type,
                          num_texels() * num_components * sizeof(type), data);
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
  template <
      typename... Components,
      typename = std::enable_if_t<sizeof...(Components) == num_components>,
      typename = std::enable_if_t<(std::is_arithmetic_v<Components> && ...)>>
  void clear(Components... components) {
    clear(std::array<type, num_components>{static_cast<type>(components)...});
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  void clear(const std::array<type, num_components>& col) {
    gl::clear_tex_image(id(), 0, gl_format, gl_type, col.data());
  }
  //------------------------------------------------------------------------------
  template <unsigned int D_ = D, typename = std::enable_if_t<D_ == 2>>
  void set_data(const pixelunpackbuffer<type>& pbo) {
    pbo.bind();
    bind();
    gl::tex_sub_image_2d(GL_TEXTURE_2D, 0, 0, 0, width(), height(), gl_format,
                         gl_type, 0);
  }
  //------------------------------------------------------------------------------
  void read(const std::string filepath) {
    auto ext = filepath.substr(filepath.find_last_of(".") + 1);
    if constexpr (D == 2 && is_readable) {
#if YAVIN_HAS_PNG_SUPPORT
      if (ext == "png") {
        read_png(filepath);
        return;
      }
#endif
    }

    throw std::runtime_error("could not read fileformat ." + ext);
  }

  //------------------------------------------------------------------------------
  void write(const std::string filepath) const {
    auto ext = filepath.substr(filepath.find_last_of(".") + 1);
    if constexpr (D == 2 && is_writable) {
#if YAVIN_HAS_PNG_SUPPORT
      if (ext == "png") {
        write_png(filepath);
        return;
      }
#endif
    }
    throw std::runtime_error("could not write fileformat ." + ext);
  }

//------------------------------------------------------------------------------
#if YAVIN_HAS_PNG_SUPPORT
template <unsigned int D_ = D, bool _PNG = has_png_support(),
          std::enable_if_t<_PNG, bool>                   = true,
          std::enable_if_t<D_ == 2 && is_readable, bool> = true>
void read_png(const std::string& filepath) {
  using tex_png_t = tex_png<type, components>;
  typename tex_png_t::png_t image;
  image.read(filepath);
  m_size[0] = image.get_width();
  m_size[1] = image.get_height();
  std::vector<type> data;
  data.reserve(num_texels() * num_components);
  for (png::uint_32 y = 0; y < height(); ++y) {
    for (png::uint_32 x = 0; x < width(); ++x) {
      tex_png_t::load_pixel(data, image, x, y);
    }
  }
  if constexpr (std::is_same_v<type, float>) {
    auto normalize = [](auto d) { return d / 255.0f; };
    boost::transform(data, begin(data), normalize);
  }

  upload_data(data);
}

//------------------------------------------------------------------------------
template <unsigned int D_ = D, bool _PNG = has_png_support(),
          std::enable_if_t<_PNG, bool>                   = true,
          std::enable_if_t<D_ == 2 && is_writable, bool> = true>
void write_png(const std::string& filepath) const {
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
#endif
};  // namespace yavin

//==============================================================================
template <typename T, typename C>
using tex1 = texture<1, T, C>;
template <typename T, typename C>
using tex2 = texture<2, T, C>;
template <typename T, typename C>
using tex3 = texture<3, T, C>;

template <typename T>
using tex1r = tex1<T, R>;
template <typename T>
using tex1rg = tex1<T, RG>;
template <typename T>
using tex1rgb = tex1<T, RGB>;
template <typename T>
using tex1rgba = tex1<T, RGBA>;
template <typename T>
using tex1bgr = tex1<T, BGR>;
template <typename T>
using tex1bgra = tex1<T, BGRA>;
template <typename T>
using tex2r = tex2<T, R>;
template <typename T>
using tex2rg = tex2<T, RG>;
template <typename T>
using tex2rgb = tex2<T, RGB>;
template <typename T>
using tex2rgba = tex2<T, RGBA>;
template <typename T>
using tex2bgr = tex2<T, BGR>;
template <typename T>
using tex2bgra = tex2<T, BGRA>;
template <typename T>
using tex2depth = tex2<T, Depth>;
template <typename T>
using tex3r = tex3<T, R>;
template <typename T>
using tex3rg = tex3<T, RG>;
template <typename T>
using tex3rgb = tex3<T, RGB>;
template <typename T>
using tex3rgba = tex3<T, RGBA>;
template <typename T>
using tex3bgr = tex3<T, BGR>;
template <typename T>
using tex3bgra = tex3<T, BGRA>;
using texdepth = tex2<unsigned int, Depth>;

using tex2r8ui  = tex2r<std::uint8_t>;
using tex2r16ui = tex2r<std::uint16_t>;
using tex2r32ui = tex2r<std::uint32_t>;
using tex2r8i   = tex2r<std::int8_t>;
using tex2r16i  = tex2r<std::int16_t>;
using tex2r32i  = tex2r<std::int32_t>;
using tex2r32f  = tex2r<float>;

using tex2rg8ui  = tex2rg<std::uint8_t>;
using tex2rg16ui = tex2rg<std::uint16_t>;
using tex2rg32ui = tex2rg<std::uint32_t>;
using tex2rg8i   = tex2rg<std::int8_t>;
using tex2rg16i  = tex2rg<std::int16_t>;
using tex2rg32i  = tex2rg<std::int32_t>;
using tex2rg32f  = tex2rg<float>;

using tex2rgb8ui  = tex2rgb<std::uint8_t>;
using tex2rgb16ui = tex2rgb<std::uint16_t>;
using tex2rgb32ui = tex2rgb<std::uint32_t>;
using tex2rgb8i   = tex2rgb<std::int8_t>;
using tex2rgb16i  = tex2rgb<std::int16_t>;
using tex2rgb32i  = tex2rgb<std::int32_t>;
using tex2rgb32f  = tex2rgb<float>;

using tex2rgba8ui  = tex2rgba<std::uint8_t>;
using tex2rgba16ui = tex2rgba<std::uint16_t>;
using tex2rgba32ui = tex2rgba<std::uint32_t>;
using tex2rgba8i   = tex2rgba<std::int8_t>;
using tex2rgba16i  = tex2rgba<std::int16_t>;
using tex2rgba32i  = tex2rgba<std::int32_t>;
using tex2rgba32f  = tex2rgba<float>;

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif