#ifndef __YAVIN_PIXEL_UNPACK_BUFFER_H__
#define __YAVIN_PIXEL_UNPACK_BUFFER_H__

#include <initializer_list>
#include <iostream>
#include <vector>
#include "Buffer.h"
#include "dll_export.h"
#include "error_check.h"
#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

template <typename T>
class PixelUnpackBuffer : public Buffer<GL_PIXEL_UNPACK_BUFFER, T> {
 public:
  using parent_t = Buffer<GL_PIXEL_UNPACK_BUFFER, T>;

  //----------------------------------------------------------------------------

  PixelUnpackBuffer() : parent_t() {}

  //----------------------------------------------------------------------------

  PixelUnpackBuffer(const PixelUnpackBuffer& other) : parent_t(other) {}

  //----------------------------------------------------------------------------

  PixelUnpackBuffer(PixelUnpackBuffer&& other) : parent_t(std::move(other)) {}

  //----------------------------------------------------------------------------

  PixelUnpackBuffer(size_t n) : parent_t(n) {}

  //----------------------------------------------------------------------------

  PixelUnpackBuffer(const std::vector<T>& data, bool direct_upload = true,
                    bool keep_data_on_cpu = false)
      : parent_t(data, direct_upload, keep_data_on_cpu) {}

  //----------------------------------------------------------------------------

  PixelUnpackBuffer(std::vector<T>&& data, bool direct_upload = true,
                    bool keep_data_on_cpu = false)
      : parent_t(std::move(data), direct_upload, keep_data_on_cpu) {}

  //----------------------------------------------------------------------------

  PixelUnpackBuffer(std::initializer_list<T>&& list)
      : parent_t(std::move(list)) {}
};

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
