#ifndef __YAVIN_VBOHELPERS_H__
#define __YAVIN_VBOHELPERS_H__

#include <array>
#include <initializer_list>
#include <iostream>
#include <vector>
#include "attribute.h"
#include "gl_includes.h"
#include "gl_type.h"

//==============================================================================
namespace Yavin {
//==============================================================================

template <size_t num_attrs, class... Ts>
struct attr_offset;

//------------------------------------------------------------------------------

template <size_t num_attrs>
struct attr_offset<num_attrs> {
  constexpr static std::array<size_t, num_attrs> gen(size_t, size_t) {
    return std::array<size_t, num_attrs>();
  }
};

//------------------------------------------------------------------------------

template <size_t num_attrs, class FirstAttrib, class... RestAttribs>
struct attr_offset<num_attrs, FirstAttrib, RestAttribs...> {
  constexpr static std::array<size_t, num_attrs> gen(size_t off, size_t idx) {
    auto arr = attr_offset<num_attrs, RestAttribs...>::gen(
        off + sizeof(FirstAttrib), idx + 1);
    arr[idx] = off;
    return arr;
  }
};

//==============================================================================

template <size_t num_attrs, class... Ts>
struct attr_generator_sizes;

//------------------------------------------------------------------------------

template <size_t num_attrs>
struct attr_generator_sizes<num_attrs> {
  constexpr static std::array<size_t, num_attrs> gen(size_t) {
    return std::array<size_t, num_attrs>();
  }
};

//------------------------------------------------------------------------------

template <size_t num_attrs, class FirstAttrib, class... RestAttribs>
struct attr_generator_sizes<num_attrs, FirstAttrib, RestAttribs...> {
  constexpr static std::array<size_t, num_attrs> gen(size_t idx) {
    auto arr = attr_generator_sizes<num_attrs, RestAttribs...>::gen(idx + 1);
    arr[idx] = sizeof(FirstAttrib);
    return arr;
  }
};

//==============================================================================

template <size_t num_attrs, class... Ts>
struct attr_generator_num_dims;

//------------------------------------------------------------------------------

template <size_t num_attrs>
struct attr_generator_num_dims<num_attrs> {
  constexpr static std::array<size_t, num_attrs> gen(size_t) {
    return std::array<size_t, num_attrs>();
  }
};

//------------------------------------------------------------------------------

template <size_t num_attrs, class FirstAttrib, class... RestAttribs>
struct attr_generator_num_dims<num_attrs, FirstAttrib, RestAttribs...> {
  constexpr static std::array<size_t, num_attrs> gen(size_t idx) {
    auto arr = attr_generator_num_dims<num_attrs, RestAttribs...>::gen(idx + 1);
    arr[idx] = FirstAttrib::num_dims;
    return arr;
  }
};

//==============================================================================

template <size_t num_attrs, class... Ts>
struct attr_generator_types;

//------------------------------------------------------------------------------

template <size_t num_attrs>
struct attr_generator_types<num_attrs> {
  constexpr static std::array<GLenum, num_attrs> gen(size_t) {
    return std::array<GLenum, num_attrs>();
  }
};

//------------------------------------------------------------------------------

template <size_t num_attrs, class FirstAttrib, class... RestAttribs>
struct attr_generator_types<num_attrs, FirstAttrib, RestAttribs...> {
  constexpr static std::array<GLenum, num_attrs> gen(size_t idx) {
    auto arr = attr_generator_types<num_attrs, RestAttribs...>::gen(idx + 1);
    arr[idx] = FirstAttrib::type;
    return arr;
  }
};

//==============================================================================

template <class... Ts>
struct attr_prefs {
  constexpr static size_t num_attrs = sizeof...(Ts);

  constexpr static std::array<size_t, num_attrs> offsets =
      attr_offset<num_attrs, Ts...>::gen(0, 0);

  constexpr static std::array<size_t, num_attrs> sizes =
      attr_generator_sizes<num_attrs, Ts...>::gen(0);

  constexpr static std::array<size_t, num_attrs> num_dims =
      attr_generator_num_dims<num_attrs, Ts...>::gen(0);

  constexpr static std::array<GLenum, num_attrs> types =
      attr_generator_types<num_attrs, Ts...>::gen(0);
};

//==============================================================================
}  // namespace Yavin
//==============================================================================
#endif
