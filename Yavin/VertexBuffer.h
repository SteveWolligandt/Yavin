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

namespace Yavin {
template <typename... Ts>
class VertexBuffer
    : public Buffer<GL_ARRAY_BUFFER, data_representation<Ts...>> {
 public:
  using parent_t = Buffer<GL_ARRAY_BUFFER, data_representation<Ts...>>;

  class iterator;
  class const_iterator;

  template <unsigned int N>
  class view;

  using data_t = data_representation<Ts...>;
  using parent_t::bind_as_copy_write_buffer;

  VertexBuffer();
  VertexBuffer(const VertexBuffer& other);
  VertexBuffer(VertexBuffer&& other);

  /**
   * @brief      constructor initializing data on cpu via lvalue
   *
   * @param[in]  data              The data
   * @param[in]  direct_upload     if true da will immediately will be uploaded
   *                               to gpu
   * @param[in]  keep_data_on_cpu  if direct upload is true, specify if data
   *                               shall be kept in cpu
   */
  VertexBuffer(const std::vector<data_t>& data, bool direct_upload = false,
               bool keep_data_on_cpu = false);

  /**
   * @brief      constructor initializing data on cpu via rvalue
   *
   * @param[in]  data              The data
   * @param[in]  direct_upload     if true da will immediately will be uploaded
   *                               to gpu
   * @param[in]  keep_data_on_cpu  if direct upload is true, specify if data
   *                               shall be kept in cpu
   */
  VertexBuffer(std::vector<data_t>&& data, bool direct_upload = false,
               bool keep_data_on_cpu = false);

  /**
   * @brief      constructor initializing data on cpu via initializer list
   *
   * @param[in]  data              The data
   * @param[in]  direct_upload     if true da will immediately will be uploaded
   *                               to gpu
   * @param[in]  keep_data_on_cpu  if direct upload is true, specify if data
   *                               shall be kept in cpu
   */
  VertexBuffer(std::initializer_list<data_t>&& list, bool direct_upload = false,
               bool keep_data_on_cpu = false);

  /**
   * @brief      uploads data to gpu
   *
   * @param[in]  keep_data_on_cpu  if true the internal data will be kept on cpu
   */
  void upload_data(bool keep_data_on_cpu = false);

  /**
   * @brief      enables and specifies vertex attribute pointer
   */
  static constexpr void activate_attributes();

  void push_back(const Ts&... ts);
  void emplace_back(const Ts&... ts);

  auto begin();
  auto end();
  auto begin() const;
  auto end() const;

  template <unsigned int N>
  auto get_view();

  template <unsigned int N>
  auto get_view() const;
  /**
   * @brief      index element-wise
   *
   * @return     returns a tuple
   */
  auto operator[](const size_t idx);
  auto operator[](const size_t idx) const;

  /**
   * index byte-wise
   */
  auto& operator()(const size_t idx);

  void download_data();

  static constexpr unsigned int num_attributes = sizeof...(Ts);
  static constexpr unsigned int size_of_attributes =
      attrib_pack_size<Ts...>::value;

 private:
  bool m_gpu_buffer_created = false;
};

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer()
    : Buffer<GL_ARRAY_BUFFER, data_representation<Ts...>>() {}

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(const VertexBuffer& other) {
  glCreateBuffers(1, &this->m_id);
  gl_error_check("glCreateBuffers");
  glCopyNamedBufferSubData(other.m_id, this->m_id, 0, 0,
                           size_of_attributes * this->gpu_size());
  gl_error_check("glCopyNamedBufferSubData");
}

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(VertexBuffer&& other)
    : Buffer<GL_ARRAY_BUFFER, data_representation<Ts...>>(std::move(other)) {}

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(const std::vector<data_t>& data,
                                  bool direct_upload, bool keep_data_on_cpu)
    : Buffer<GL_ARRAY_BUFFER, data_representation<Ts...>>(data, direct_upload,
                                                          keep_data_on_cpu) {}

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(std::vector<data_t>&& data,
                                  bool direct_upload, bool keep_data_on_cpu)
    : Buffer<GL_ARRAY_BUFFER, data_representation<Ts...>>(
          std::move(data), direct_upload, keep_data_on_cpu) {}

template <typename... Ts>
VertexBuffer<Ts...>::VertexBuffer(std::initializer_list<data_t>&& list,
                                  bool direct_upload, bool keep_data_on_cpu)
    : Buffer<GL_ARRAY_BUFFER, data_representation<Ts...>>(
          std::move(list), direct_upload, keep_data_on_cpu) {}

template <typename... Ts>
void VertexBuffer<Ts...>::upload_data(bool keep_data_on_cpu) {
  // gpu buffer neiter created nor initialized
  if (!m_gpu_buffer_created) {
    glNamedBufferData(this->m_id, size_of_attributes * this->cpu_size(),
                      this->m_data.data(), GL_STATIC_DRAW);
    gl_error_check("glNamedBufferData");

    this->m_gpu_size = this->cpu_size();

    this->m_is_consistent = true;
    m_gpu_buffer_created  = true;

    // gpu buffer already present
  } else {
    auto gpu_buffer = reinterpret_cast<unsigned char*>(
        glMapNamedBuffer(this->m_id, GL_WRITE_ONLY));
    gl_error_check("glMapNamedBuffer");

    this->m_data.resize(this->m_gpu_size);
    unsigned char* tmp_cpu_buffer =
        reinterpret_cast<unsigned char*>(this->m_data.data());
    for (size_t i = 0; i < this->m_gpu_size * attrib_pack_size<Ts...>::value;
         ++i) {
      gpu_buffer[i] = tmp_cpu_buffer[i];
    }

    glUnmapNamedBuffer(this->m_id);
    gl_error_check("glUnmapNamedBuffer");
    this->m_is_consistent = true;
  }

  if (!keep_data_on_cpu)
    this->clear();
  else
    this->m_is_consistent = false;
}

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

template <typename... Ts>
void VertexBuffer<Ts...>::push_back(const Ts&... ts) {
  this->m_is_consistent = false;
  this->m_data.push_back(data_t(ts...));
}

template <typename... Ts>
void VertexBuffer<Ts...>::emplace_back(const Ts&... ts) {
  this->m_is_consistent = false;
  this->m_data.emplace_back(ts...);
}

template <typename... Ts>
auto VertexBuffer<Ts...>::begin() {
  return iterator(*this, 0);
}

template <typename... Ts>
auto VertexBuffer<Ts...>::end() {
  return iterator(*this, this->cpu_size());
}

template <typename... Ts>
auto VertexBuffer<Ts...>::begin() const {
  return const_iterator(*this, 0);
}

template <typename... Ts>
auto VertexBuffer<Ts...>::end() const {
  return const_iterator(*this, this->cpu_size());
}

template <typename... Ts>
template <unsigned int N>
auto VertexBuffer<Ts...>::get_view() {
  return view<N>(*this);
}

template <typename... Ts>
template <unsigned int N>
auto VertexBuffer<Ts...>::get_view() const {
  return view<N>(*this);
}

template <typename... Ts>
auto VertexBuffer<Ts...>::operator[](const size_t idx) {
  return tuple_constructor<data_representation<Ts...>, sizeof...(Ts), 0,
                           Yavin::attrib_pack_size<Ts...>::value,
                           Ts...>::construct(this->m_data, idx,
                                             Yavin::attr_prefs<Ts...>::offsets);
}

template <typename... Ts>
auto VertexBuffer<Ts...>::operator[](const size_t idx) const {
  return tuple_constructor<data_representation<Ts...>, sizeof...(Ts), 0,
                           Yavin::attrib_pack_size<Ts...>::value,
                           Ts...>::construct(this->m_data, idx,
                                             Yavin::attr_prefs<Ts...>::offsets);
}

template <typename... Ts>
auto& VertexBuffer<Ts...>::operator()(const size_t idx) {
  return reinterpret_cast<unsigned char*>(this->m_data.data())[idx];
}

template <typename... Ts>
void VertexBuffer<Ts...>::download_data() {
  auto gpu_buffer = reinterpret_cast<unsigned char*>(
      glMapNamedBuffer(this->m_id, GL_READ_ONLY));
  gl_error_check("glMapNamedBuffer");
  this->m_data.resize(this->m_gpu_size);
  unsigned char* tmp_cpu_buffer =
      reinterpret_cast<unsigned char*>(this->m_data.data());
  for (size_t i = 0; i < this->m_gpu_size * attrib_pack_size<Ts...>::value; ++i)
    tmp_cpu_buffer[i] = gpu_buffer[i];
  glUnmapNamedBuffer(this->m_id);
  gl_error_check("glUnmapNamedBuffer");
  this->m_is_consistent = true;
}

template <typename... Ts>
class VertexBuffer<Ts...>::iterator {
 public:
  iterator(VertexBuffer<Ts...>& vbo, const size_t idx)
      : m_vbo(vbo), m_idx(idx) {}

  iterator(const iterator& other) : m_vbo(other.m_vbo), m_idx(other.m_idx) {}
  // iterator& operator=(iterator& other) {
  //   m_vbo = other.m_vbo;
  //   m_idx = other.m_idx;
  // }

  bool operator==(const iterator& other) { return m_idx == other.m_idx; }

  bool operator!=(const iterator& other) { return m_idx != other.m_idx; }

  iterator& operator++() { m_idx += 1; }

  std::tuple<Ts&...> operator*() const { return m_vbo[m_idx]; }

  VertexBuffer<Ts...>& m_vbo;
  size_t               m_idx = 0;
};

template <typename... Ts>
class VertexBuffer<Ts...>::const_iterator {
 public:
  const_iterator(const VertexBuffer<Ts...>& vbo, const size_t idx)
      : m_vbo(vbo), m_idx(idx) {}

  const_iterator(const const_iterator& other)
      : m_vbo(other.m_vbo), m_idx(other.m_idx) {}
  // const_iterator& operator=(const_iterator& other) {
  //   m_vbo = other.m_vbo;
  //   m_idx = other.m_idx;
  // }

  bool operator==(const const_iterator& other) { return m_idx == other.m_idx; }

  bool operator!=(const const_iterator& other) { return m_idx != other.m_idx; }

  const_iterator& operator++() { m_idx += 1; }

  const std::tuple<const Ts&...> operator*() const { return m_vbo[m_idx]; }

  const VertexBuffer<Ts...>& m_vbo;
  size_t                     m_idx = 0;
};

template <typename... Ts>
template <unsigned int N>
class VertexBuffer<Ts...>::view {
 public:
  class iterator;
  class const_iterator;

  view(VertexBuffer<Ts...>& vbo) : m_vbo(vbo) {}

  static constexpr unsigned int offset = Yavin::attr_prefs<Ts...>::offsets[N];

  auto& operator[](const size_t idx) {
    return *reinterpret_cast<typename index_param_pack<N, Ts...>::type*>(
        &m_vbo(idx * Yavin::attrib_pack_size<Ts...>::value) +
        Yavin::attr_prefs<Ts...>::offsets[N]);
  }

  auto begin() { return iterator(*this, 0); }
  auto end() { return iterator(*this, m_vbo.cpu_size()); }

  auto begin() const { return const_iterator(*this, 0); }
  auto end() const { return const_iterator(*this, m_vbo.cpu_size()); }

  VertexBuffer& m_vbo;
};

template <typename... Ts>
template <unsigned int N>
class VertexBuffer<Ts...>::view<N>::iterator {
 public:
  iterator(VertexBuffer<Ts...>::view<N>& vbo_view, const size_t idx)
      : m_vbo_view(vbo_view), m_idx(idx) {}

  iterator(const iterator& other)
      : m_vbo_view(other.m_vbo_view), m_idx(other.m_idx) {}

  // iterator& operator=(iterator& other) {
  //   m_vbo_view = other.m_vbo_view;
  //   m_idx      = other.m_idx;
  // }

  bool operator==(const iterator& other) { return m_idx == other.m_idx; }

  bool operator!=(const iterator& other) { return m_idx != other.m_idx; }

  iterator& operator++() { m_idx += 1; }

  auto& operator*() const { return m_vbo_view[m_idx]; }

  VertexBuffer<Ts...>::view<N>& m_vbo_view;
  size_t                        m_idx = 0;
};

template <typename... Ts>
template <unsigned int N>
class VertexBuffer<Ts...>::view<N>::const_iterator {
 public:
  const_iterator(const VertexBuffer<Ts...>::view<N>& vbo_view, const size_t idx)
      : m_vbo_view(vbo_view), m_idx(idx) {}

  const_iterator(const const_iterator& other)
      : m_vbo_view(other.m_vbo_view), m_idx(other.m_idx) {}

  // const_iterator& operator=(const_iterator& other) {
  //   m_vbo_view = other.m_vbo_view;
  //   m_idx      = other.m_idx;
  // }

  bool operator==(const const_iterator& other) { return m_idx == other.m_idx; }

  bool operator!=(const const_iterator& other) { return m_idx != other.m_idx; }

  const_iterator& operator++() { m_idx += 1; }

  const auto& operator*() const { return m_vbo_view[m_idx]; }

  const VertexBuffer<Ts...>::view<N>& m_vbo_view;
  size_t                              m_idx = 0;
};
}  // namespace Yavin

#endif
