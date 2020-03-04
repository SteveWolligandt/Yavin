#ifndef YAVIN_BUFFER
#define YAVIN_BUFFER
//==============================================================================
#include <mutex>
#include <vector>
#include "glfunctions.h"
#include "mutexhandler.h"
#include "idholder.h"
//==============================================================================
namespace yavin {
//==============================================================================
template <GLsizei array_type, typename T>
class buffer;
//==============================================================================
template <GLsizei ArrayType, typename T, GLbitfield Access>
class buffer_map {
 public:
  static constexpr auto access     = Access;
  static constexpr auto array_type = ArrayType;
  using buffer_t                   = buffer<array_type, T>;
  static constexpr auto data_size  = buffer_t::data_size;

  /// constructor gets a mapping to gpu_buffer
  buffer_map(const buffer_t* buffer, size_t offset, size_t length)
      : m_buffer(buffer), m_offset(offset), m_length(length) {
    m_gpu_mapping = (T*)gl::map_named_buffer_range(
        m_buffer->id(), data_size * offset, data_size * m_length,
        access);
    detail::mutex::gl_call.lock();
  }

  /// destructor unmaps the buffer
  ~buffer_map() { unmap(); }

  void unmap() {
    detail::mutex::gl_call.unlock();
    if (!m_unmapped) {
      gl::unmap_named_buffer(m_buffer->id());
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

 protected:
  const buffer_t* m_buffer;
  size_t          m_offset;
  size_t          m_length;
  T*              m_gpu_mapping;
  bool            m_unmapped = false;
};

template <GLsizei array_type, typename T>
using rbuffer_map = buffer_map<array_type, T, GL_MAP_READ_BIT>;

template <GLsizei array_type, typename T>
using wbuffer_map = buffer_map<array_type, T, GL_MAP_WRITE_BIT>;

template <GLsizei array_type, typename T>
using rwbuffer_map =
    buffer_map<array_type, T, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT>;

//==============================================================================
/// Returned by buffer::operator[] const for reading single elements
template <GLsizei array_type, typename T>
class readable_buffer_element {
 public:
  using buffer_t = buffer<array_type, T>;
  using rmap_t   = rbuffer_map<array_type, T>;

  readable_buffer_element(const buffer_t* buffer, size_t idx)
      : m_buffer(buffer), m_idx(idx) {}
  readable_buffer_element(const readable_buffer_element& other)
      : m_buffer(other.m_buffer), m_idx(other.m_idx) {}
  readable_buffer_element(readable_buffer_element&& other)
      : m_buffer(other.m_buffer), m_idx(other.m_idx) {}

  /// for accessing single gpu data element.
  operator T() const { return download(); }

  auto download() const {
    rmap_t map(m_buffer, m_idx, 1);
    return map.front();
  }

 protected:
  const buffer_t* m_buffer;
  size_t          m_idx;
};

template <GLsizei array_type, typename T>
inline auto& operator<<(std::ostream&                         out,
                        readable_buffer_element<array_type, T>& data) {
  out << data.download();
  return out;
}

//==============================================================================
/// Returned by buffer::operator[] for reading and writing single elements
template <GLsizei array_type, typename T>
class writeable_buffer_element : public readable_buffer_element<array_type, T> {
 public:
  using parent_t    = readable_buffer_element<array_type, T>;
  using buffer_t    = typename parent_t::buffer_t;
  using wmap_wmap_t = wbuffer_map<array_type, T>;

  writeable_buffer_element(buffer_t* buffer, size_t idx)
      : parent_t(buffer, idx) {}
  writeable_buffer_element(const writeable_buffer_element& other)
      : parent_t(other) {}
  writeable_buffer_element(writeable_buffer_element&& other)
      : parent_t(std::move(other)) {}

  /// for assigning single gpu data element.
  auto& operator=(T&& data) {
    gl::named_buffer_sub_data(this->m_buffer->id(),
                              this->m_idx * buffer_t::data_size,
                              buffer_t::data_size, &data);
    return *this;
  }
};

//------------------------------------------------------------------------------

template <GLsizei array_type, typename T>
inline auto& operator<<(std::ostream&                          out,
                        writeable_buffer_element<array_type, T>& data) {
  out << data.download();
  return out;
}

//==============================================================================
/// non-const buffer iterator
template <GLsizei array_type, typename T>
class buffer_iterator {
 public:
  using buffer_t = buffer<array_type, T>;
  //----------------------------------------------------------------------------
  // iterator typedefs
  using value_type        = T;
  using reference         = T&;
  using pointer           = T*;
  using difference_type   = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;
  //----------------------------------------------------------------------------
  buffer_iterator(buffer_t* buffer, size_t idx) : m_buffer(buffer), m_idx(idx) {}
  //----------------------------------------------------------------------------
  buffer_iterator(const buffer_iterator& other)
      : m_buffer(other.m_buffer), m_idx(other.m_idx) {}
  //----------------------------------------------------------------------------
  buffer_iterator(buffer_iterator&& other)
      : m_buffer(other.m_buffer), m_idx(other.m_idx) {}
  //----------------------------------------------------------------------------
  auto& operator=(const buffer_iterator& other) {
    m_buffer = other.m_buffer;
    m_idx    = other.m_idx;
    return *this;
  }
  //----------------------------------------------------------------------------
  auto& operator=(buffer_iterator&& other) {
    m_buffer = other.m_buffer;
    m_idx    = other.m_idx;
    return *this;
  }
  //----------------------------------------------------------------------------
  /// get the buffer element the iterator refers to
  T operator*() const { return readable_buffer_element(m_buffer, m_idx); }
  //----------------------------------------------------------------------------
  /// are two iterators equal?
  bool operator==(const buffer_iterator& other) const {
    return (m_idx == other.m_idx);
  }
  //----------------------------------------------------------------------------
  /// are two iterators different?
  bool operator!=(const buffer_iterator& other) const {
    return !operator==(other);
  }
  //----------------------------------------------------------------------------
  /// pre-increment iterator
  auto& operator++() {
    ++m_idx;
    return *this;
  }
  //----------------------------------------------------------------------------
  /// post-increment iterator
  auto operator++(int) {
    buffer_iterator vi{*this};
    ++(*this);
    return vi;
  }
  //----------------------------------------------------------------------------
  /// pre-decrement iterator
  auto& operator--() {
    --m_idx;
    return *this;
  }
  //----------------------------------------------------------------------------
  /// post-decrement iterator
  auto operator--(int) {
    buffer_iterator vi(*this);
    --(*this);
    return vi;
  }

 private:
  buffer_t* m_buffer;
  size_t    m_idx;
};

//==============================================================================
/// const buffer iterator
template <GLsizei array_type, typename T>
class cbuffer_iterator {
 public:
  using buffer_t = buffer<array_type, T>;

  // iterator typedefs
  using value_type        = T;
  using reference         = T&;
  using pointer           = T*;
  using difference_type   = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;
  //----------------------------------------------------------------------------
  cbuffer_iterator(const buffer_t* buffer, size_t idx)
      : m_buffer(buffer), m_idx(idx) {}
  //----------------------------------------------------------------------------
  cbuffer_iterator(const cbuffer_iterator& other)
      : m_buffer(other.m_buffer), m_idx(other.m_idx) {}
  //----------------------------------------------------------------------------
  cbuffer_iterator(cbuffer_iterator&& other)
      : m_buffer(other.m_buffer), m_idx(other.m_idx) {}
  //----------------------------------------------------------------------------
  auto& operator=(const cbuffer_iterator& other) {
    m_buffer = other.m_buffer;
    m_idx    = other.m_idx;
    return *this;
  }
  //----------------------------------------------------------------------------
  auto& operator=(cbuffer_iterator&& other) {
    m_buffer = other.m_buffer;
    m_idx    = other.m_idx;
    return *this;
  }
  //----------------------------------------------------------------------------
  /// get the buffer element the iterator refers to
  T operator*() const { return readable_buffer_element(m_buffer, m_idx); }
  //----------------------------------------------------------------------------
  /// are two iterators equal?
  bool operator==(const cbuffer_iterator& other) const {
    return (m_idx == other.m_idx);
  }
  //----------------------------------------------------------------------------
  /// are two iterators different?
  bool operator!=(const cbuffer_iterator& other) const {
    return !operator==(other);
  }
  //----------------------------------------------------------------------------
  /// pre-increment iterator
  auto& operator++() {
    ++m_idx;
    return *this;
  }
  //----------------------------------------------------------------------------
  /// post-increment iterator
  auto operator++(int) {
    cbuffer_iterator vi(*this);
    ++(*this);
    return vi;
  }
  //----------------------------------------------------------------------------
  /// pre-decrement iterator
  auto& operator--() {
    --m_idx;
    return *this;
  }
  //----------------------------------------------------------------------------
  /// post-decrement iterator
  auto operator--(int) {
    cbuffer_iterator vi(*this);
    --(*this);
    return vi;
  }

 private:
  const buffer_t* m_buffer;
  size_t          m_idx;
};


//==============================================================================
/// buffer base class for each of the OpenGL buffer types
template <GLsizei _array_type, typename T>
class buffer : public id_holder<GLuint> {
 public:
  using parent_t = id_holder<GLuint>;
  using parent_t::id;
  friend class readable_buffer_element<_array_type, T>;
  friend class writeable_buffer_element<_array_type, T>;

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

  using this_t = buffer<array_type, T>;
  using data_t = T;

  using element_t       = writeable_buffer_element<array_type, T>;
  using const_element_t = readable_buffer_element<array_type, T>;

  using iterator_t       = buffer_iterator<array_type, T>;
  using const_iterator_t = cbuffer_iterator<array_type, T>;

  using rmap_t  = rbuffer_map<array_type, T>;
  using wmap_t  = wbuffer_map<array_type, T>;
  using rwmap_t = rwbuffer_map<array_type, T>;

 protected:
  size_t  m_size     = 0;
  size_t  m_capacity = 0;
  usage_t m_usage;

 public:
  buffer(usage_t usage);
  buffer(const buffer& other);
  buffer(buffer&& other);
  buffer& operator=(const buffer& other);
  buffer& operator=(buffer&& other);

  buffer(size_t n, usage_t usage);
  buffer(size_t n, const T& initial, usage_t usage);
  buffer(const std::vector<T>& data, usage_t usage);
  ~buffer();

  void create_handle();
  void destroy_handle();

  void           upload_data(const std::vector<T>& data);
  std::vector<T> download_data() const;

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

  auto wmap() { return wmap_t(this, 0, m_size); }

  auto rmap() { return rmap_t(this, 0, m_size); }
  auto rmap() const { return rap_t(this, 0, m_size); }

  auto map() { return rwmap_t(this, 0, m_size); }
  auto map() const { return rmap_t(this, 0, m_size); }

  auto map(size_t offset, size_t length) {
    return rwmap_t(this, offset, length);
  }
  auto map(size_t offset, size_t length) const {
    return rmap_t(this, offset, length);
  }
};
//==============================================================================
template <GLsizei array_type, typename T>
buffer<array_type, T>::buffer(usage_t usage)
    : m_size{}, m_capacity{}, m_usage(usage) {
  create_handle();
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
buffer<array_type, T>::buffer(const buffer& other) : buffer(other.m_usage) {
  m_usage = other.m_usage;
  copy(other);
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
buffer<array_type, T>::buffer(buffer&& other)
    : parent_t{std::move(other)},
      m_size(std::exchange(other.m_size, 0)),
      m_capacity(std::exchange(other.m_capacity, 0)),
      m_usage(other.m_usage) {}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
buffer<array_type, T>& buffer<array_type, T>::operator=(const buffer& other) {
  m_usage = other.m_usage;
  copy(other);
  return *this;
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
buffer<array_type, T>& buffer<array_type, T>::operator=(buffer&& other) {
  parent_t::operator=(std::move(other));
  std::swap(m_size, other.m_size);
  std::swap(m_capacity, other.m_capacity);
  std::swap(m_usage, other.m_usage);
  return *this;
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
buffer<array_type, T>::buffer(size_t n, usage_t usage) : buffer(usage) {
  gpu_malloc(n);
  m_size = n;
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
buffer<array_type, T>::buffer(size_t n, const T& initial, usage_t usage)
    : buffer(usage) {
  gpu_malloc(n, initial);
  m_size = n;
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
buffer<array_type, T>::buffer(const std::vector<T>& data, usage_t usage)
    : buffer(usage) {
  upload_data(data);
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
buffer<array_type, T>::~buffer() {
  destroy_handle();
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
void buffer<array_type, T>::create_handle() {
  gl::create_buffers(1, &id());
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
void buffer<array_type, T>::destroy_handle() {
  if (id() != 0) gl::delete_buffers(1, &id());
  id() = 0;
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
void buffer<array_type, T>::upload_data(const std::vector<T>& data) {
  if (capacity() < data.size()) {
    // reallocate new memory
    gl::named_buffer_data(id(), data_size * data.size(), data.data(),
                          m_usage);
    m_size = m_capacity = data.size();

  } else {
    // just update buffer
    gl::named_buffer_sub_data(id(), 0, data_size * data.size(),
                              data.data());
    m_size = data.size();
  }
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
void buffer<array_type, T>::reserve(size_t size) {
  if (capacity() < size) {
    this_t tmp(*this);
    gpu_malloc(size);
    copy(tmp);
  }
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
void buffer<array_type, T>::resize(size_t size) {
  if (capacity() < size) { reserve(size); }
  m_size = size;
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
std::vector<T> buffer<array_type, T>::download_data() const {
  rmap_t         map(this, 0, size());
  std::vector<T> data(size());
  std::copy(map.begin(), map.end(), data.begin());
  return data;
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
void buffer<array_type, T>::gpu_malloc(size_t n) {
  gl::named_buffer_data<void>(this->id(), data_size * n, nullptr, m_usage);
  m_capacity = n;
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
void buffer<array_type, T>::gpu_malloc(size_t n, const T& initial) {
  std::vector<T> data(n, initial);
  gl::named_buffer_data(this->id(), data_size * n, data.data(), m_usage);
  m_capacity = n;
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
void buffer<array_type, T>::bind() const {
  gl::bind_buffer(array_type, id());
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
void buffer<array_type, T>::unbind() {
  gl::bind_buffer(array_type, 0);
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
void buffer<array_type, T>::copy(const this_t& other) {
  if (capacity() < other.size()) gpu_malloc(other.size());
  gl::copy_named_buffer_sub_data(other.id(), id(), 0, 0,
                                 data_size * other.size());
  m_size = other.size();
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
void buffer<array_type, T>::push_back(T&& t) {
  if (m_capacity < m_size + 1) reserve(m_size * 2);
  at(size()) = std::forward<T>(t);
  ++m_size;
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
void buffer<array_type, T>::pop_back() {
  --m_size;
}
//------------------------------------------------------------------------------
template <GLsizei array_type, typename T>
template <typename... Ts>
void buffer<array_type, T>::emplace_back(Ts&&... ts) {
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
}  // namespace yavin
//==============================================================================
#endif
