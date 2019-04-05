#ifndef __YAVIN_PIXEL_UNPACK_BUFFER_H__
#define __YAVIN_PIXEL_UNPACK_BUFFER_H__

#include <initializer_list>
#include <iostream>
#include <vector>
#include "buffer.h"
#include "dll_export.h"
#include "error_check.h"
#include "gl_includes.h"

//==============================================================================
namespace yavin {
//==============================================================================

template <typename T>
class PixelUnpackBuffer : public Buffer<GL_PIXEL_UNPACK_BUFFER, T> {
 public:
  using parent_t                     = Buffer<GL_PIXEL_UNPACK_BUFFER, T>;
  using this_t                       = PixelUnpackBuffer<T>;
  using usage_t                      = typename parent_t::usage_t;
  static const usage_t default_usage = usage_t::STATIC_COPY;

  //----------------------------------------------------------------------------

  PixelUnpackBuffer(usage_t usage = default_usage) : parent_t(usage) {}
  PixelUnpackBuffer(const PixelUnpackBuffer& other) : parent_t(other) {}
  PixelUnpackBuffer(PixelUnpackBuffer&& other) : parent_t(std::move(other)) {}

  auto& operator=(const PixelUnpackBuffer& other) {
    parent_t::operator=(other);
    return *this;
  }

  auto& operator=(PixelUnpackBuffer&& other) {
    parent_t::operator=(std::move(other));
    return *this;
  }

  PixelUnpackBuffer(size_t n, usage_t usage = default_usage)
      : parent_t(n, usage) {}
  PixelUnpackBuffer(size_t n, const T& initial, usage_t usage = default_usage)
      : parent_t(n, initial, usage) {}
  PixelUnpackBuffer(const std::vector<T>& data, usage_t usage = default_usage)
      : parent_t(data, usage) {}
  PixelUnpackBuffer(std::initializer_list<T>&& list)
      : parent_t(std::move(list), default_usage) {}
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
