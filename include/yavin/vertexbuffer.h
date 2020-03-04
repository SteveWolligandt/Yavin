#ifndef __YAVIN_VERTEXBUFFER_H__
#define __YAVIN_VERTEXBUFFER_H__

#include <initializer_list>
#include <iostream>
#include <tuple>
#include <vector>
#include "buffer.h"
#include "errorcheck.h"
#include "glincludes.h"
#include "tuple.h"
#include "vbohelpers.h"

//==============================================================================
namespace yavin {
//==============================================================================

template <typename... Ts>
class vertexbuffer : public buffer<GL_ARRAY_BUFFER, tuple<Ts...>> {
 public:
  using parent_t = buffer<GL_ARRAY_BUFFER, tuple<Ts...>>;
  using this_t   = vertexbuffer<Ts...>;
  using data_t   = typename parent_t::data_t;
  using usage_t  = typename parent_t::usage_t;

  static constexpr size_t data_size = parent_t::data_size;

  static const usage_t default_usage = usage_t::STATIC_DRAW;

  static constexpr unsigned int num_attributes = sizeof...(Ts);
  static constexpr std::array<size_t, num_attributes> num_components{Ts::num_components()...};
  static constexpr std::array<GLenum, num_attributes> types{Ts::type...};
  static constexpr std::array<size_t, num_attributes> offsets =
      attr_offset<num_attributes, Ts...>::gen(0, 0);

  //----------------------------------------------------------------------------

  vertexbuffer(usage_t usage = default_usage) : parent_t(usage) {}
  vertexbuffer(const vertexbuffer& other) : parent_t(other) {}
  vertexbuffer(vertexbuffer&& other) : parent_t(other) {}

  auto& operator=(const this_t& other) {
    parent_t::operator=(other);
    return *this;
  }

  auto& operator=(this_t&& other) {
    parent_t::operator=(std::move(other));
    return *this;
  }

  vertexbuffer(size_t n, usage_t usage = default_usage) : parent_t(n, usage) {}
  vertexbuffer(size_t n, const data_t& initial, usage_t usage = default_usage)
      : parent_t(n, initial, usage) {}
  vertexbuffer(const std::vector<data_t>& data, usage_t usage = default_usage)
      : parent_t(data, usage) {}
  vertexbuffer(std::initializer_list<data_t>&& list)
      : parent_t(std::move(list), default_usage) {}

  void push_back(Ts&&... ts) {
    parent_t::push_back(make_tuple(std::forward<Ts>(ts)...));
  }

  static constexpr void activate_attributes() {
    for (unsigned int i = 0; i < num_attributes; i++) {
      gl::enable_vertex_attrib_array(i);
      gl::vertex_attrib_pointer(i, num_components[i], types[i], GL_FALSE,
                                this_t::data_size, (void*)offsets[i]);
    }
  }

  static constexpr void deactivate_attributes() {
    for (unsigned int i = 0; i < num_attributes; i++)
      gl::disable_vertex_attrib_array(i);
  }
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
