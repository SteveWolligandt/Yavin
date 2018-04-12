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
  using parent_t::gpu_malloc;
  using usage_t = typename parent_t::usage_t;
  using data_t  = data_representation<Ts...>;

  static constexpr unsigned int num_attributes = sizeof...(Ts);
  static constexpr unsigned int size_of_attributes =
      attrib_pack_size<Ts...>::value;
  static const usage_t default_usage = usage_t::STATIC_DRAW;

  static constexpr void activate_attributes();

  VertexBuffer(usage_t usage = default_usage);
  VertexBuffer(const VertexBuffer& other);
  VertexBuffer(VertexBuffer&& other);

  VertexBuffer(const std::vector<data_t>& data, usage_t usage = default_usage,
               bool direct_upload = true, bool keep_data_on_cpu = false);

  VertexBuffer(std::vector<data_t>&& data, usage_t usage = default_usage,
               bool direct_upload = true, bool keep_data_on_cpu = false);

  VertexBuffer(std::initializer_list<data_t>&& list);

  void copy(const this_t& other);

 private:
  bool m_gpu_buffer_created = false;
};

//------------------------------------------------------------------------------

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(usage_t usage) : parent_t(usage) {}

//------------------------------------------------------------------------------

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(const VertexBuffer& other)
    : parent_t(other.m_usage) {
  this->m_size     = other.m_size;
  this->m_capacity = other.m_capacity;
  this->m_delete   = other.m_delete;
  copy(other);
}

//------------------------------------------------------------------------------

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(VertexBuffer&& other)
    : parent_t(std::move(other)) {}

//------------------------------------------------------------------------------

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(const std::vector<data_t>& data,
                                  usage_t usage, bool direct_upload,
                                  bool keep_data_on_cpu)
    : parent_t(data, usage, direct_upload, keep_data_on_cpu) {}

//------------------------------------------------------------------------------

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(std::vector<data_t>&& data, usage_t usage,
                                  bool direct_upload, bool keep_data_on_cpu)
    : parent_t(std::move(data), usage, direct_upload, keep_data_on_cpu) {}

//------------------------------------------------------------------------------

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(std::initializer_list<data_t>&& list)
    : parent_t(std::move(list), default_usage) {}

//------------------------------------------------------------------------------

template <typename... Ts>
void VertexBuffer<Ts...>::copy(const this_t& other) {
  gpu_malloc(other.capacity());
  glCopyNamedBufferSubData(other.m_id, this->m_id, 0, 0,
                           size_of_attributes * this->capacity());
  gl_error_check("glCopyNamedBufferSubData");
}

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
