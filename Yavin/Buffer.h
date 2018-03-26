#ifndef __YAVIN_GPUBUFFER__
#define __YAVIN_GPUBUFFER__

#include <vector>
#include "error_check.h"
#include "gl_includes.h"

namespace Yavin {
template <class T1, class... T>
struct first {
  typedef T1 type;
};

template <int _array_type, typename T>
class Buffer {
 public:
  constexpr static int array_type = _array_type;

  Buffer();
  Buffer(const Buffer& other);
  Buffer(Buffer&& other);
  Buffer(const std::vector<T>& data, bool direct_upload = false,
         bool keep_data_on_cpu = false);
  Buffer(std::vector<T>&& data, bool direct_upload = false,
         bool keep_data_on_cpu = false);
  Buffer(std::initializer_list<T>&& list, bool direct_upload = false,
         bool keep_data_on_cpu = false);
  ~Buffer();

  void upload_data(bool keep_data_on_cpu = false);

  void        bind() const;
  void        bind_as_copy_write_buffer() const;
  static void unbind();
  static void unbind_as_copy_write_buffer();

  auto cpu_size() const;
  auto gpu_size() const;
  auto is_consistent() const;

  void reserve(const size_t size);
  void clear();

  void push_back(const T& data);
  void push_back(const std::initializer_list<T>& data);
  template <typename S>
  void push_back(const std::initializer_list<S>& data);

  // template <typename... Ts>
  // void push_back(const Ts&... ts);
  void emplace_back(T data);

  void set_data(const std::vector<T>& data);
  void set_data(std::vector<T>&& data);
  void set_data(std::initializer_list<T>&& l);
  void download_data();

  auto&       operator[](size_t index) { return m_data[index]; }
  const auto& operator[](size_t index) const { return m_data[index]; }

  typename std::vector<T>::iterator       begin() { return m_data.begin(); }
  typename std::vector<T>::iterator       end() { return m_data.end(); }
  typename std::vector<T>::const_iterator begin() const {
    return m_data.begin();
  }
  typename std::vector<T>::const_iterator end() const { return m_data.end(); }

 protected:
  unsigned int   m_id;
  size_t         m_gpu_size      = 0;
  bool           m_is_consistent = false;
  bool           m_dont_delete   = false;
  std::vector<T> m_data;
};

template <int array_type, typename T>
Buffer<array_type, T>::Buffer() {
  glCreateBuffers(1, &m_id);
  gl_error_check("glCreateBuffers");
}

template <int array_type, typename T>
Buffer<array_type, T>::Buffer(const Buffer& other)
    : m_is_consistent(other.m_is_consistent), m_data(other.m_data) {
  glCreateBuffers(1, &m_id);
  gl_error_check("glCreateBuffers");
  glCopyNamedBufferSubData(other.m_id, m_id, 0, 0, sizeof(T) * gpu_size());
  gl_error_check("glCopyNamedBufferSubData");
}

template <int array_type, typename T>
Buffer<array_type, T>::Buffer(Buffer&& other)
    : m_id(other.m_id),
      m_gpu_size(other.m_gpu_size),
      m_is_consistent(other.m_is_consistent),
      m_data(std::move(other.m_data)) {
  other.m_dont_delete = true;
}

template <int array_type, typename T>
Buffer<array_type, T>::Buffer(const std::vector<T>& data, bool direct_upload,
                              bool keep_data_on_cpu)
    : m_data(data) {
  glCreateBuffers(1, &m_id);
  gl_error_check("glCreateBuffers");
  if (direct_upload) upload_data(keep_data_on_cpu);
}

template <int array_type, typename T>
Buffer<array_type, T>::Buffer(std::vector<T>&& data, bool direct_upload,
                              bool keep_data_on_cpu)
    : m_data(std::move(data)) {
  glCreateBuffers(1, &m_id);
  gl_error_check("glCreateBuffers");
  if (direct_upload) upload_data(keep_data_on_cpu);
}

template <int array_type, typename T>
Buffer<array_type, T>::Buffer(std::initializer_list<T>&& list,
                              bool direct_upload, bool keep_data_on_cpu)
    : m_data(std::move(list)) {
  glCreateBuffers(1, &m_id);
  gl_error_check("glCreateBuffers");
  if (direct_upload) upload_data(keep_data_on_cpu);
}

template <int array_type, typename T>
Buffer<array_type, T>::~Buffer() {
  glDeleteBuffers(1, &m_id);
  gl_error_check("glDeleteBuffers");
}

template <int array_type, typename T>
void Buffer<array_type, T>::upload_data(bool keep_data_on_cpu) {
  if (!m_is_consistent) {
    glNamedBufferData(m_id, sizeof(T) * cpu_size(), m_data.data(),
                      GL_STATIC_DRAW);
    gl_error_check("glNamedBufferData");
    m_gpu_size = cpu_size();

    m_is_consistent = true;
  }

  if (!keep_data_on_cpu)
    clear();
  else
    m_is_consistent = false;
}

template <int array_type, typename T>
void Buffer<array_type, T>::bind() const {
  glBindBuffer(array_type, m_id);
  gl_error_check("glBindBuffer");
}

template <int array_type, typename T>
void Buffer<array_type, T>::bind_as_copy_write_buffer() const {
  glBindBuffer(GL_COPY_WRITE_BUFFER, m_id);
  gl_error_check("glBindBuffer");
}

template <int array_type, typename T>
void Buffer<array_type, T>::unbind() {
  glBindBuffer(array_type, 0);
  gl_error_check("glBindBuffer");
}

template <int array_type, typename T>
void Buffer<array_type, T>::unbind_as_copy_write_buffer() {
  glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
  gl_error_check("glBindBuffer");
}

template <int array_type, typename T>
auto Buffer<array_type, T>::cpu_size() const {
  return m_data.size();
}

template <int array_type, typename T>
auto Buffer<array_type, T>::gpu_size() const {
  return m_gpu_size;
}

template <int array_type, typename T>
void Buffer<array_type, T>::push_back(const T& idx) {
  m_is_consistent = false;
  m_data.push_back(idx);
}

template <int array_type, typename T>
void Buffer<array_type, T>::push_back(const std::initializer_list<T>& data) {
  m_is_consistent = false;
  for (const auto& date : data) m_data.push_back(date);
}

template <int array_type, typename T>
template <typename S>
void Buffer<array_type, T>::push_back(const std::initializer_list<S>& data) {
  m_is_consistent = false;
  for (const auto& date : data) m_data.push_back(static_cast<T>(date));
}

// template <int array_type, typename T>
// template <typename... Ts>
// void Buffer<array_type, T>::push_back(const Ts&... ts) {
//   push_back(std::initializer_list<const typename first<Ts...>::type>(ts...));
// }

template <int array_type, typename T>
void Buffer<array_type, T>::emplace_back(T idx) {
  m_is_consistent = false;
  m_data.emplace_back(idx);
}

template <int array_type, typename T>
void Buffer<array_type, T>::set_data(const std::vector<T>& data) {
  m_is_consistent = false;
  m_data          = data;
}

template <int array_type, typename T>
void Buffer<array_type, T>::set_data(std::vector<T>&& data) {
  m_is_consistent = false;
  m_data          = std::move(data);
}

template <int array_type, typename T>
void Buffer<array_type, T>::set_data(std::initializer_list<T>&& l) {
  m_is_consistent = false;
  m_data          = std::move(l);
}

template <int array_type, typename T>
auto Buffer<array_type, T>::is_consistent() const {
  return m_is_consistent;
}

template <int array_type, typename T>
void Buffer<array_type, T>::reserve(const size_t size) {
  m_data.reserve(size);
}

template <int array_type, typename T>
void Buffer<array_type, T>::clear() {
  m_data.clear();
}

template <int array_type, typename T>
void Buffer<array_type, T>::download_data() {
  auto gpu_buffer = reinterpret_cast<unsigned char*>(
      glMapNamedBuffer(this->m_id, GL_READ_ONLY));
  m_data.resize(m_gpu_size);
  unsigned char* tmp_buffer = reinterpret_cast<unsigned char*>(m_data.data());
  for (size_t i = 0; i < m_gpu_size * sizeof(T); ++i)
    tmp_buffer[i] = gpu_buffer[i];
  glUnmapNamedBuffer(this->m_id);
  m_is_consistent = true;
}
}  // namespace Yavin

#endif
