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
class VertexBuffer
    : public Buffer<GL_ARRAY_BUFFER, data_representation<Ts...>> {
 public:
  using parent_t = Buffer<GL_ARRAY_BUFFER, data_representation<Ts...>>;
  using this_t   = VertexBuffer<Ts...>;
  using usage_t  = typename parent_t::usage_t;
  using data_t   = data_representation<Ts...>;

  static constexpr unsigned int num_attributes = sizeof...(Ts);
  static const usage_t          default_usage  = usage_t::STATIC_DRAW;

  static constexpr void activate_attributes();

  VertexBuffer(usage_t usage = default_usage);
  VertexBuffer(const VertexBuffer& other);
  VertexBuffer(VertexBuffer&& other);

  VertexBuffer(const std::vector<data_t>& data, usage_t usage = default_usage);
  VertexBuffer(std::initializer_list<data_t>&& list);
};

//------------------------------------------------------------------------------

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(usage_t usage) : parent_t(usage) {}

//------------------------------------------------------------------------------

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(const VertexBuffer& other)
    : parent_t(other) {}

//------------------------------------------------------------------------------

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(VertexBuffer&& other)
    : parent_t(std::move(other)) {}

//------------------------------------------------------------------------------

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(const std::vector<data_t>& data,
                                  usage_t                    usage)
    : parent_t(data, usage) {}

//------------------------------------------------------------------------------

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(std::initializer_list<data_t>&& list)
    : parent_t(std::move(list), default_usage) {}

//------------------------------------------------------------------------------

template <typename... Ts>
constexpr void VertexBuffer<Ts...>::activate_attributes() {
  for (unsigned int i = 0; i < attr_prefs<Ts...>::num_attrs; i++) {
    glEnableVertexAttribArray(i);
    gl_error_check("glEnableVertexAttribArray");
    glVertexAttribPointer(i, attr_prefs<Ts...>::num_dims[i],
                          attr_prefs<Ts...>::types[i], GL_FALSE,
                          attrib_pack_size<Ts...>::value,
                          (void*)attr_prefs<Ts...>::offsets[i]);
    gl_error_check("glVertexAttribPointer");
  }
}

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
