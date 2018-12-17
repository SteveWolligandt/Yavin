#ifndef __YAVIN_GPUBUFFER__
#define __YAVIN_GPUBUFFER__

#include <mutex>
#include <vector>
#include "gl_functions.h"
#include "mutex_handler.h"

//==============================================================================
namespace Yavin {
//==============================================================================

template <GLsizei array_type, typename T>
class Buffer;

//==============================================================================

template <GLsizei array_type, typename T>
class BufferMap {
 public:
  using buffer_t                    = Buffer<array_type, T>;
  constexpr static size_t data_size = buffer_t::data_size;

  //! constructor gets a mapping to gpu_buffer
  BufferMap(const buffer_t* buffer, size_t offset, size_t length,
            GLbitfield access)
      : m_buffer(buffer), m_offset(offset), m_length(length), m_access(access) {
    m_gpu_mapping = (T*)gl::map_named_buffer_range(
        m_buffer->m_gl_handle, data_size * offset, data_size * m_length,
        m_access);
    detail::mutex::gl_call.lock();
  }

  //! destructor unmaps the buffer
  ~BufferMap() { unmap(); }

  void unmap() {
    detail::mutex::gl_call.unlock();
    if (!m_unmapped) {
      gl::unmap_named_buffer(m_buffer->m_gl_handle);
      m_unmapped = true;
    }
  }

  auto&       at(size_t i) { return m_gpu_mapping[i]; }
  const auto& at(size_t i) const { return m_gpu_mapping[i]; }

  auto&       front() { return at(0); }
  const auto& front() const { return at(0); }

  auto&       back() { return at(m_length - 1); }
  const auto& back() const { return at(m_length - 1); }

  auto&       operator[](size_t i) { return at(i); }
  const auto& operator[](size_t i) const { return at(i); }

  auto begin() { return m_gpu_mapping; }
  auto end() { return m_gpu_mapping + m_length; }
  auto begin() const { return m_gpu_mapping; }
  auto end() const { return m_gpu_mapping + m_length; }

  auto offset() const { return m_offset; }
  auto length() const { return m_length; }
  auto access() const { return m_access; }

 protected:
  const buffer_t* m_buffer;
  size_t          m_offset;
  size_t          m_length;
  GLbitfield      m_access;
  T*              m_gpu_mapping;
  bool            m_unmapped = false;
};

template <GLsizei array_type, typename T>
class RBufferMap : public BufferMap<array_type, T> {
 public:
  RBufferMap(const Buffer<array_type, T>* buffer, size_t offset, size_t length)
      : BufferMap<array_type, T>(buffer, offset, length, GL_MAP_READ_BIT) {}
};

template <GLsizei array_type, typename T>
class WBufferMap : public BufferMap<array_type, T> {
 public:
  WBufferMap(Buffer<array_type, T>* buffer, size_t offset, size_t length)
      : BufferMap<array_type, T>(buffer, offset, length, GL_MAP_WRITE_BIT) {}
};

template <GLsizei array_type, typename T>
class RWBufferMap : public BufferMap<array_type, T> {
 public:
  RWBufferMap(Buffer<array_type, T>* buffer, size_t offset, size_t length)
      : BufferMap<array_type, T>(buffer, offset, length,
                                 GL_MAP_WRITE_BIT | GL_MAP_READ_BIT) {}
};

//==============================================================================
//! Returned by Buffer::operator[] const for reading single elements
template <GLsizei array_type, typename T>
class ReadableBufferElement {
 public:
  using buffer_t = Buffer<array_type, T>;
  using r_map_t  = RBufferMap<array_type, T>;

  ReadableBufferElement(const buffer_t* buffer, size_t idx)
      : m_buffer(buffer), m_idx(idx) {}
  ReadableBufferElement(const ReadableBufferElement& other)
      : m_buffer(other.m_buffer), m_idx(other.m_idx) {}
  ReadableBufferElement(ReadableBufferElement&& other)
      : m_buffer(other.m_buffer), m_idx(other.m_idx) {}

  //! for accessing single gpu data element.
  operator T() const { return download(); }

  auto download() const {
    r_map_t map(m_buffer, m_idx, 1);
    return map.front();
  }

 protected:
  const buffer_t* m_buffer;
  size_t          m_idx;
};

template <GLsizei array_type, typename T>
inline auto& operator<<(std::ostream&                         out,
                        ReadableBufferElement<array_type, T>& data) {
  out << data.download();
  return out;
}

//==============================================================================
//! Returned by Buffer::operator[] for reading and writing single elements
template <GLsizei array_type, typename T>
class WriteableBufferElement : public ReadableBufferElement<array_type, T> {
 public:
  using parent_t = ReadableBufferElement<array_type, T>;
  using buffer_t = typename parent_t::buffer_t;
  using w_map_t  = WBufferMap<array_type, T>;

  WriteableBufferElement(buffer_t* buffer, size_t idx)
      : parent_t(buffer, idx) {}
  WriteableBufferElement(const WriteableBufferElement& other)
      : parent_t(other) {}
  WriteableBufferElement(WriteableBufferElement&& other)
      : parent_t(std::move(other)) {}

  //! for assigning single gpu data element.
  auto& operator=(T&& data) {
    gl::named_buffer_sub_data(this->m_buffer->m_gl_handle,
                              this->m_idx * buffer_t::data_size,
                              buffer_t::data_size, &data);
    return *this;
  }
};

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
inline auto& operator<<(std::ostream&                          out,
                        WriteableBufferElement<array_type, T>& data) {
  out << data.download();
  return out;
}

//==============================================================================
//! non-const buffer iterator
template <GLsizei array_type, typename T>
class BufferIterator {
 public:
  using buffer_t = Buffer<array_type, T>;

  //----------------------------------------------------------------------------

  // iterator typedefs
  using value_type        = T;
  using reference         = T&;
  using pointer           = T*;
  using difference_type   = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

  //----------------------------------------------------------------------------

  BufferIterator(buffer_t* buffer, size_t idx) : m_buffer(buffer), m_idx(idx) {}

  //----------------------------------------------------------------------------

  BufferIterator(const BufferIterator& other)
      : m_buffer(other.m_buffer), m_idx(other.m_idx) {}

  //----------------------------------------------------------------------------

  BufferIterator(BufferIterator&& other)
      : m_buffer(other.m_buffer), m_idx(other.m_idx) {}

  //----------------------------------------------------------------------------

  auto& operator=(const BufferIterator& other) {
    m_buffer = other.m_buffer;
    m_idx    = other.m_idx;
    return *this;
  }

  //----------------------------------------------------------------------------

  auto& operator=(BufferIterator&& other) {
    m_buffer = other.m_buffer;
    m_idx    = other.m_idx;
    return *this;
  }

  //----------------------------------------------------------------------------
  //! get the buffer element the iterator refers to
  T operator*() const { return ReadableBufferElement(m_buffer, m_idx); }

  //----------------------------------------------------------------------------
  //! are two iterators equal?
  bool operator==(const BufferIterator& other) const {
    return (m_idx == other.m_idx);
  }

  //----------------------------------------------------------------------------
  //! are two iterators different?
  bool operator!=(const BufferIterator& other) const {
    return !operator==(other);
  }

  //----------------------------------------------------------------------------
  //! pre-increment iterator
  auto& operator++() {
    ++m_idx;
    return *this;
  }

  //----------------------------------------------------------------------------
  //! post-increment iterator
  auto operator++(int) {
    BufferIterator vi(*this);
    ++(*this);
    return vi;
  }

  //----------------------------------------------------------------------------
  //! pre-decrement iterator
  auto& operator--() {
    --m_idx;
    return *this;
  }

  //----------------------------------------------------------------------------
  //! post-decrement iterator
  auto operator--(int) {
    BufferIterator vi(*this);
    --(*this);
    return vi;
  }

 private:
  buffer_t* m_buffer;
  size_t    m_idx;
};

//==============================================================================
//! const buffer iterator
template <GLsizei array_type, typename T>
class ConstBufferIterator {
 public:
  using buffer_t = Buffer<array_type, T>;

  // iterator typedefs
  using value_type        = T;
  using reference         = T&;
  using pointer           = T*;
  using difference_type   = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

  //----------------------------------------------------------------------------

  ConstBufferIterator(const buffer_t* buffer, size_t idx)
      : m_buffer(buffer), m_idx(idx) {}

  //----------------------------------------------------------------------------

  ConstBufferIterator(const ConstBufferIterator& other)
      : m_buffer(other.m_buffer), m_idx(other.m_idx) {}

  //----------------------------------------------------------------------------

  ConstBufferIterator(ConstBufferIterator&& other)
      : m_buffer(other.m_buffer), m_idx(other.m_idx) {}

  //----------------------------------------------------------------------------

  auto& operator=(const ConstBufferIterator& other) {
    m_buffer = other.m_buffer;
    m_idx    = other.m_idx;
    return *this;
  }

  //----------------------------------------------------------------------------

  auto& operator=(ConstBufferIterator&& other) {
    m_buffer = other.m_buffer;
    m_idx    = other.m_idx;
    return *this;
  }

  //----------------------------------------------------------------------------
  //! get the buffer element the iterator refers to
  T operator*() const { return ReadableBufferElement(m_buffer, m_idx); }

  //----------------------------------------------------------------------------
  //! are two iterators equal?
  bool operator==(const ConstBufferIterator& other) const {
    return (m_idx == other.m_idx);
  }

  //----------------------------------------------------------------------------
  //! are two iterators different?
  bool operator!=(const ConstBufferIterator& other) const {
    return !operator==(other);
  }

  //----------------------------------------------------------------------------
  //! pre-increment iterator
  auto& operator++() {
    ++m_idx;
    return *this;
  }

  //----------------------------------------------------------------------------
  //! post-increment iterator
  auto operator++(int) {
    ConstBufferIterator vi(*this);
    ++(*this);
    return vi;
  }

  //----------------------------------------------------------------------------
  //! pre-decrement iterator
  auto& operator--() {
    --m_idx;
    return *this;
  }

  //----------------------------------------------------------------------------
  //! post-decrement iterator
  auto operator--(int) {
    ConstBufferIterator vi(*this);
    --(*this);
    return vi;
  }

 private:
  const buffer_t* m_buffer;
  size_t          m_idx;
};

//==============================================================================

//==============================================================================
/**
 * Buffer base class for each of the OpenGL buffer types
 */
template <GLsizei _array_type, typename T>
class Buffer {
  friend class ReadableBufferElement<_array_type, T>;
  friend class WriteableBufferElement<_array_type, T>;
  friend class BufferMap<_array_type, T>;

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

  constexpr static GLsizei array_type = _array_type;
  constexpr static size_t  data_size  = sizeof(T);

  using this_t = Buffer<array_type, T>;
  using data_t = T;

  using element_t       = WriteableBufferElement<array_type, T>;
  using const_element_t = ReadableBufferElement<array_type, T>;

  using iterator_t       = BufferIterator<array_type, T>;
  using const_iterator_t = ConstBufferIterator<array_type, T>;

  using map_t    = BufferMap<array_type, T>;
  using r_map_t  = RBufferMap<array_type, T>;
  using w_map_t  = WBufferMap<array_type, T>;
  using rw_map_t = RWBufferMap<array_type, T>;

  Buffer(usage_t usage);
  Buffer(const Buffer& other);
  Buffer(Buffer&& other);
  Buffer& operator=(const Buffer& other);
  Buffer& operator=(Buffer&& other);

  Buffer(size_t n, usage_t usage);
  Buffer(size_t n, const T& initial, usage_t usage);
  Buffer(const std::vector<T>& data, usage_t usage);
  ~Buffer();

  void create_handle();
  void destroy_handle();

  void           upload_data(const std::vector<T>& data);
  std::vector<T> download_data();

  void        bind() const;
  static void unbind();

  void copy(const this_t& other);

  auto size() const { return m_size; }
  auto capacity() const { return m_capacity; }

  void reserve(size_t size);
  void resize(size_t size);
  void clear() { m_size = 0; }

  void gpu_malloc(size_t n);
  void gpu_malloc(size_t n, const T& initial);

  void push_back(T&&);
  void pop_back();

  template <typename... Ts>
  void emplace_back(Ts&&...);

  auto at(size_t idx) { return element_t(this, idx); }
  auto at(size_t idx) const { return const_element_t(this, idx); }
  auto operator[](size_t idx) { return at(idx); }
  auto operator[](size_t idx) const { return at(idx); }

  auto front() { return at(0); }
  auto front() const { return at(0); }
  auto back() { return at(m_size - 1); }
  auto back() const { return at(m_size - 1); }

  auto begin() { return iterator_t(this, 0); }
  auto end() { return iterator_t(this, m_size); }

  auto begin() const { return const_iterator_t(this, 0); }
  auto end() const { return const_iterator_t(this, m_size); }

  auto gl_handle() { return m_gl_handle; }

  auto map() { return rw_map_t(this, 0, m_size); }
  auto map() const { return r_map_t(this, 0, m_size); }

  auto map(size_t offset, size_t length) {
    return rw_map_t(this, offset, length);
  }
  auto map(size_t offset, size_t length) const {
    return r_map_t(this, offset, length);
  }

 protected:
  GLuint  m_gl_handle;
  size_t  m_size     = 0;
  size_t  m_capacity = 0;
  usage_t m_usage;
};

//==============================================================================

template <GLsizei array_type, typename T>
Buffer<array_type, T>::Buffer(usage_t usage) : m_usage(usage) {
  create_handle();
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
Buffer<array_type, T>::Buffer(const Buffer& other) : Buffer(other.m_usage) {
  m_usage = other.m_usage;
  copy(other);
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
Buffer<array_type, T>::Buffer(Buffer&& other)
    : m_gl_handle(std::exchange(other.m_gl_handle, 0)),
      m_size(std::exchange(other.m_size, 0)),
      m_capacity(std::exchange(other.m_capacity, 0)),
      m_usage(other.m_usage) {}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
Buffer<array_type, T>& Buffer<array_type, T>::operator=(const Buffer& other) {
  m_usage = other.m_usage;
  copy(other);
  return *this;
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
Buffer<array_type, T>& Buffer<array_type, T>::operator=(Buffer&& other) {
  std::swap(m_gl_handle, other.m_gl_handle);
  std::swap(m_size, other.m_size);
  std::swap(m_capacity, other.m_capacity);
  std::swap(m_usage, other.m_usage);
  return *this;
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
Buffer<array_type, T>::Buffer(size_t n, usage_t usage) : Buffer(usage) {
  gpu_malloc(n);
  m_size = n;
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
Buffer<array_type, T>::Buffer(size_t n, const T& initial, usage_t usage)
    : Buffer(usage) {
  gpu_malloc(n, initial);
  m_size = n;
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
Buffer<array_type, T>::Buffer(const std::vector<T>& data, usage_t usage)
    : Buffer(usage) {
  upload_data(data);
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
Buffer<array_type, T>::~Buffer() {
  destroy_handle();
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
void Buffer<array_type, T>::create_handle() {
  gl::create_buffers(1, &m_gl_handle);
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
void Buffer<array_type, T>::destroy_handle() {
  if (m_gl_handle != 0) gl::delete_buffers(1, &m_gl_handle);
  m_gl_handle = 0;
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
void Buffer<array_type, T>::upload_data(const std::vector<T>& data) {
  if (capacity() < data.size()) {
    // reallocate new memory
    gl::named_buffer_data(m_gl_handle, data_size * data.size(), data.data(),
                          m_usage);
    m_size = m_capacity = data.size();

  } else {
    // just update buffer
    gl::named_buffer_sub_data(m_gl_handle, 0, data_size * data.size(),
                              data.data());
    m_size = data.size();
  }
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
void Buffer<array_type, T>::reserve(size_t size) {
  if (capacity() < size) {
    this_t tmp(*this);
    gpu_malloc(size);
    copy(tmp);
  }
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
void Buffer<array_type, T>::resize(size_t size) {
  if (capacity() < size) reserve(size);
  m_size = size;
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
std::vector<T> Buffer<array_type, T>::download_data() {
  r_map_t        map(this, 0, size());
  std::vector<T> data(size());
  for (size_t i = 0; i < size(); ++i) data[i] = map[i];
  return data;
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
void Buffer<array_type, T>::gpu_malloc(size_t n) {
  gl::named_buffer_data(this->m_gl_handle, data_size * n, nullptr, m_usage);
  m_capacity = n;
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
void Buffer<array_type, T>::gpu_malloc(size_t n, const T& initial) {
  std::vector<T> data(n, initial);
  gl::named_buffer_data(this->m_gl_handle, data_size * n, data.data(), m_usage);
  m_capacity = n;
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
void Buffer<array_type, T>::bind() const {
  gl::bind_buffer(array_type, m_gl_handle);
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
void Buffer<array_type, T>::unbind() {
  gl::bind_buffer(array_type, 0);
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
void Buffer<array_type, T>::copy(const this_t& other) {
  if (capacity() < other.size()) gpu_malloc(other.size());
  gl::copy_named_buffer_sub_data(other.m_gl_handle, m_gl_handle, 0, 0,
                                 data_size * other.size());
  m_size = other.size();
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
void Buffer<array_type, T>::push_back(T&& t) {
  if (m_capacity < m_size + 1) reserve(m_size * 2);
  at(size()) = std::forward<T>(t);
  ++m_size;
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
void Buffer<array_type, T>::pop_back() {
  --m_size;
}

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
template <typename... Ts>
void Buffer<array_type, T>::emplace_back(Ts&&... ts) {
  if (m_capacity < m_size + 1) {
    // reallocate
    this_t tmp(*this);
    gpu_malloc(m_size * 2.0);
    copy(tmp);
  }
  at(size()) = T(std::forward<Ts>(ts)...);
  ++m_size;
}

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
