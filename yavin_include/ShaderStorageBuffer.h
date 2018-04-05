#ifndef __YAVIN_SHADER_STORAGE_BUFFER_H__
#define __YAVIN_SHADER_STORAGE_BUFFER_H__

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

  ShaderStorageBuffer(const std::vector<T>& data, usage_t usage = default_usage,
                      bool direct_upload = true, bool keep_data_on_cpu = false)
      : parent_t(data, usage, direct_upload, keep_data_on_cpu) {}

  //----------------------------------------------------------------------------

  ShaderStorageBuffer(std::vector<T>&& data, usage_t usage = default_usage,
                      bool direct_upload = true, bool keep_data_on_cpu = false)
      : parent_t(std::move(data), usage, direct_upload, keep_data_on_cpu) {}

  //----------------------------------------------------------------------------

  ShaderStorageBuffer(std::initializer_list<T>&& list)
      : parent_t(std::move(list)) {}

  //----------------------------------------------------------------------------

  void bind(size_t index) const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, this->m_id);
    gl_error_check("glBindBufferBase");
  }
};

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
