#ifndef __YAVIN_GPUBUFFER__
#define __YAVIN_GPUBUFFER__

#include <vector>
#include "error_check.h"
#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

template <class T1, class... T>
struct first {
  typedef T1 type;
};

//==============================================================================

template <int _array_type, typename T>
class Buffer {
 public:
  enum usage_t {
    STREAM_DRAW  = GL_STREAM_DRAW,
    STREAM_READ  = GL_STREAM_READ,
    STREAM_COPY  = GL_STREAM_COPY,
    STATIC_DRAW  = GL_STATIC_DRAW,
    STATIC_READ  = GL_STATIC_READ,
    STATIC_COPY  = GL_STATIC_COPY,
    DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
    DYNAMIC_READ = GL_DYNAMIC_READ,
    DYNAMIC_COPY = GL_DYNAMIC_COPY
  };
  using this_t                    = Buffer<_array_type, T>;
  constexpr static int array_type = _array_type;

  Buffer(usage_t usage);
  Buffer(const Buffer& other);
  Buffer(Buffer&& other);
  this_t& operator=(const Buffer& other);
  this_t& operator=(Buffer&& other);
  Buffer(size_t n, usage_t usage);
  Buffer(const std::vector<T>& data, usage_t usage, bool direct_upload = true,
         bool keep_data_on_cpu = false);
  Buffer(std::vector<T>&& data, usage_t usage, bool direct_upload = true,
         bool keep_data_on_cpu = false);
  Buffer(std::initializer_list<T>&& list, usage_t usage);
  ~Buffer();

  void upload_data(bool keep_data_on_cpu = false);

  void gpu_malloc_bytes(size_t bytes);
  void gpu_malloc(size_t n);
  void gpu_malloc();

  void        bind() const;
  static void unbind();

  void        bind_as_copy_write_buffer() const;
  static void unbind_as_copy_write_buffer();

  void copy(const this_t& other);

  auto cpu_size() const { return m_data.size(); }
  auto gpu_size() const { return m_gpu_size; }
  auto is_consistent() const { return m_is_consistent; }

  void reserve(const size_t size) { m_data.reserve(size); }
  void clear() { m_data.clear(); }

  void push_back(const T& data);
  void push_back(const std::initializer_list<T>& data);
  template <typename S>
  void push_back(const std::initializer_list<S>& data);

  void emplace_back(T data);

  auto&       front() { return m_data.front(); }
  const auto& front() const { return m_data.front(); }
  auto&       back() { return m_data.back(); }
  const auto& back() const { return m_data.back(); }

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
  usage_t        m_usage;
  std::vector<T> m_data;
};

//==============================================================================

template <int array_type, typename T>
Buffer<array_type, T>::Buffer(usage_t usage) : m_usage(usage) {
  glCreateBuffers(1, &m_id);
  gl_error_check("glCreateBuffers");
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
Buffer<array_type, T>::Buffer(const Buffer& other)
    : m_gpu_size(other.m_gpu_size),
      m_is_consistent(other.m_is_consistent),
      m_dont_delete(other.m_dont_delete),
      m_usage(other.m_usage),
      m_data(other.m_data) {
  glCreateBuffers(1, &m_id);
  gl_error_check("glCreateBuffers");
  copy(other);
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
Buffer<array_type, T>::Buffer(Buffer&& other)
    : m_id(other.m_id),
      m_gpu_size(other.m_gpu_size),
      m_is_consistent(other.m_is_consistent),
      m_usage(other.m_usage),
      m_data(std::move(other.m_data)) {
  other.m_dont_delete = true;
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
Buffer<array_type, T>& Buffer<array_type, T>::operator=(const Buffer& other) {
  m_gpu_size      = other.m_gpu_size;
  m_is_consistent = other.m_is_consistent;
  m_dont_delete   = other.m_dont_delete;
  m_usage         = other.usage;
  m_data          = other.m_data;
  glCreateBuffers(1, &m_id);
  gl_error_check("glCreateBuffers");
  copy(other);
  return *this;
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
Buffer<array_type, T>& Buffer<array_type, T>::operator=(Buffer&& other) {
  m_id                = other.m_id;
  m_gpu_size          = other.m_gpu_size;
  m_is_consistent     = other.m_is_consistent;
  m_usage             = other.usage;
  m_data              = std::move(other.m_data);
  other.m_dont_delete = true;
  return *this;
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
Buffer<array_type, T>::Buffer(size_t n, usage_t usage) : Buffer(usage) {
  gpu_malloc(n);
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
Buffer<array_type, T>::Buffer(const std::vector<T>& data, usage_t usage,
                              bool direct_upload, bool keep_data_on_cpu)
    : m_usage(usage), m_data(data) {
  glCreateBuffers(1, &m_id);
  gl_error_check("glCreateBuffers");
  if (direct_upload) upload_data(keep_data_on_cpu);
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
Buffer<array_type, T>::Buffer(std::vector<T>&& data, usage_t usage,
                              bool direct_upload, bool keep_data_on_cpu)
    : m_usage(usage), m_data(std::move(data)) {
  glCreateBuffers(1, &m_id);
  gl_error_check("glCreateBuffers");
  if (direct_upload) upload_data(keep_data_on_cpu);
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
Buffer<array_type, T>::Buffer(std::initializer_list<T>&& list, usage_t usage)
    : m_usage(usage), m_data(std::move(list)) {
  glCreateBuffers(1, &m_id);
  gl_error_check("glCreateBuffers");
  upload_data(false);
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
Buffer<array_type, T>::~Buffer() {
  glDeleteBuffers(1, &m_id);
  gl_error_check("glDeleteBuffers");
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::upload_data(bool keep_data_on_cpu) {
  glNamedBufferData(m_id, sizeof(T) * cpu_size(), m_data.data(), m_usage);
  gl_error_check("glNamedBufferData");
  m_gpu_size = cpu_size();

  m_is_consistent = true;

  if (!keep_data_on_cpu)
    clear();
  else
    m_is_consistent = false;
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::gpu_malloc_bytes(size_t bytes) {
  glNamedBufferData(this->m_id, bytes, nullptr, m_usage);
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::gpu_malloc(size_t n) {
  m_gpu_size = n;
  gpu_malloc_bytes(n * sizeof(T));
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::gpu_malloc() {
  gpu_malloc(cpu_size());
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::bind() const {
  glBindBuffer(array_type, m_id);
  gl_error_check("glBindBuffer");
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::bind_as_copy_write_buffer() const {
  glBindBuffer(GL_COPY_WRITE_BUFFER, m_id);
  gl_error_check("glBindBuffer");
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::unbind() {
  glBindBuffer(array_type, 0);
  gl_error_check("glBindBuffer");
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::unbind_as_copy_write_buffer() {
  glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
  gl_error_check("glBindBuffer");
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::copy(const this_t& other) {
  gpu_malloc(other.gpu_size());
  glCopyNamedBufferSubData(other.m_id, m_id, 0, 0, sizeof(T) * gpu_size());
  gl_error_check("glCopyNamedBufferSubData");
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::push_back(const T& idx) {
  m_is_consistent = false;
  m_data.push_back(idx);
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::push_back(const std::initializer_list<T>& data) {
  m_is_consistent = false;
  for (const auto& date : data) m_data.push_back(date);
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
template <typename S>
void Buffer<array_type, T>::push_back(const std::initializer_list<S>& data) {
  m_is_consistent = false;
  for (const auto& date : data) m_data.push_back(static_cast<T>(date));
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::emplace_back(T idx) {
  m_is_consistent = false;
  m_data.emplace_back(idx);
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::set_data(const std::vector<T>& data) {
  m_is_consistent = false;
  m_data          = data;
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::set_data(std::vector<T>&& data) {
  m_is_consistent = false;
  m_data          = std::move(data);
}

//------------------------------------------------------------------------------

template <int array_type, typename T>
void Buffer<array_type, T>::set_data(std::initializer_list<T>&& l) {
  m_is_consistent = false;
  m_data          = std::move(l);
}

//------------------------------------------------------------------------------

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

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
