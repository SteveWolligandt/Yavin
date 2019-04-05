#ifndef __YAVIN_SHADER_STORAGE_BUFFER_H__
#define __YAVIN_SHADER_STORAGE_BUFFER_H__

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
class ShaderStorageBuffer : public Buffer<GL_SHADER_STORAGE_BUFFER, T> {
 public:
  using parent_t                     = Buffer<GL_SHADER_STORAGE_BUFFER, T>;
  using usage_t                      = typename parent_t::usage_t;
  static const usage_t default_usage = usage_t::DYNAMIC_DRAW;

  //----------------------------------------------------------------------------

  ShaderStorageBuffer(usage_t usage = default_usage) : parent_t(usage) {}

  //----------------------------------------------------------------------------

  ShaderStorageBuffer(const ShaderStorageBuffer& other) : parent_t(other) {}

  //----------------------------------------------------------------------------

  ShaderStorageBuffer(ShaderStorageBuffer&& other)
      : parent_t(std::move(other)) {}

  //----------------------------------------------------------------------------

  ShaderStorageBuffer(size_t n, usage_t usage = default_usage)
      : parent_t(n, usage) {}

  //----------------------------------------------------------------------------

  ShaderStorageBuffer(size_t n, const T& initial, usage_t usage = default_usage)
      : parent_t(n, initial, usage) {}

  //----------------------------------------------------------------------------

  ShaderStorageBuffer(const std::vector<T>& data, usage_t usage = default_usage)
      : parent_t(data, usage) {}

  //----------------------------------------------------------------------------

  ShaderStorageBuffer(std::initializer_list<T>&& list)
      : parent_t(std::move(list), default_usage) {}

  //----------------------------------------------------------------------------

  void bind(size_t index) const {
    gl::bind_buffer_base(GL_SHADER_STORAGE_BUFFER, index, this->m_gl_handle);
  }

  //----------------------------------------------------------------------------

  static void unbind(size_t index) {
    gl::bind_buffer_base(GL_SHADER_STORAGE_BUFFER, index, 0);
  }
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif