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
  constexpr static auto gen(size_t, size_t) {
    return std::array<size_t, num_attrs>();
  }
};

//------------------------------------------------------------------------------

template <size_t num_attrs, class FirstAttrib, class... RestAttribs>
struct attr_offset<num_attrs, FirstAttrib, RestAttribs...> {
  constexpr static auto gen(size_t off = 0, size_t idx = 0) {
    auto arr = attr_offset<num_attrs, RestAttribs...>::gen(
        off + sizeof(FirstAttrib), idx + 1);
    arr[idx] = off;
    return arr;
  }
};

//==============================================================================
}  // namespace Yavin
//==============================================================================
#endif
