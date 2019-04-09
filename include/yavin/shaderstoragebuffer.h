#ifndef __YAVIN_SHADER_STORAGE_BUFFER_H__
#define __YAVIN_SHADER_STORAGE_BUFFER_H__

#include <initializer_list>
#include <iostream>
#include <vector>
#include "buffer.h"
#include "dllexport.h"
#include "errorcheck.h"
#include "glincludes.h"

//==============================================================================
namespace yavin {
//==============================================================================

template <typename T>
class shaderstoragebuffer : public buffer<GL_SHADER_STORAGE_BUFFER, T> {
 public:
  using parent_t                     = buffer<GL_SHADER_STORAGE_BUFFER, T>;
  using usage_t                      = typename parent_t::usage_t;
  static const usage_t default_usage = usage_t::DYNAMIC_DRAW;

  //----------------------------------------------------------------------------

  shaderstoragebuffer(usage_t usage = default_usage) : parent_t(usage) {}

  //----------------------------------------------------------------------------

  shaderstoragebuffer(const shaderstoragebuffer& other) : parent_t(other) {}

  //----------------------------------------------------------------------------

  shaderstoragebuffer(shaderstoragebuffer&& other)
      : parent_t(std::move(other)) {}

  //----------------------------------------------------------------------------

  shaderstoragebuffer(size_t n, usage_t usage = default_usage)
      : parent_t(n, usage) {}

  //----------------------------------------------------------------------------

  shaderstoragebuffer(size_t n, const T& initial, usage_t usage = default_usage)
      : parent_t(n, initial, usage) {}

  //----------------------------------------------------------------------------

  shaderstoragebuffer(const std::vector<T>& data, usage_t usage = default_usage)
      : parent_t(data, usage) {}

  //----------------------------------------------------------------------------

  shaderstoragebuffer(std::initializer_list<T>&& list)
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
