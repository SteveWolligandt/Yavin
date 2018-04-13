#ifndef __YAVIN_VERTEXBUFFER_H__
#define __YAVIN_VERTEXBUFFER_H__

#include <initializer_list>
#include <iostream>
#include <tuple>
#include <vector>
#include "Buffer.h"
#include "VBOHelpers.h"
#include "error_check.h"
#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

template <typename... Ts>
class VertexBuffer : public Buffer<GL_ARRAY_BUFFER, std::tuple<Ts...>> {
 public:
  using parent_t = Buffer<GL_ARRAY_BUFFER, std::tuple<Ts...>>;
  using this_t   = VertexBuffer<Ts...>;
  using data_t   = typename parent_t::data_t;
  using usage_t  = typename parent_t::usage_t;

  static constexpr unsigned int num_attributes = sizeof...(Ts);
  static const usage_t          default_usage  = usage_t::STATIC_DRAW;

  //----------------------------------------------------------------------------

  VertexBuffer(usage_t usage = default_usage) : parent_t(usage) {}
  VertexBuffer(const VertexBuffer& other) : parent_t(other) {}
  VertexBuffer(VertexBuffer&& other) : parent_t(other) {}

  auto& operator=(const this_t& other) {
    parent_t::operator=(other);
    return *this;
  }

  auto& operator=(this_t&& other) {
    parent_t::operator=(std::move(other));
    return *this;
  }

  VertexBuffer(size_t n, usage_t usage = default_usage) : parent_t(n, usage) {}
  VertexBuffer(size_t n, const data_t& initial, usage_t usage = default_usage)
      : parent_t(n, initial, usage) {}
  VertexBuffer(const std::vector<data_t>& data, usage_t usage = default_usage)
      : parent_t(data, usage) {}
  VertexBuffer(std::initializer_list<data_t>&& list)
      : parent_t(std::move(list), default_usage) {}

  void push_back(Ts&&... ts) {
    parent_t::push_back(std::make_tuple(std::forward<Ts>(ts)...));
  }

  static constexpr void activate_attributes() {
    for (unsigned int i = 0; i < attr_prefs<Ts...>::num_attrs; i++) {
      glEnableVertexAttribArray(i);
      gl_error_check("glEnableVertexAttribArray");
      glVertexAttribPointer(
          i, attr_prefs<Ts...>::num_dims[i], attr_prefs<Ts...>::types[i],
          GL_FALSE, this_t::data_size, (void*)attr_prefs<Ts...>::offsets[i]);
      gl_error_check("glVertexAttribPointer");
    }
  }
};

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
