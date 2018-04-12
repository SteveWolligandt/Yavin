#include "AtomicCounterBuffer.h"

#include <cstring>
#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

AtomicCounterBuffer::AtomicCounterBuffer(usage_t usage) : Buffer(usage) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(const AtomicCounterBuffer& other)
    : Buffer(other) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(AtomicCounterBuffer&& other)
    : Buffer(std::move(other)) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer& AtomicCounterBuffer::operator=(
    const AtomicCounterBuffer& other) {
  parent_t::operator=(other);
  return *this;
}

//------------------------------------------------------------------------------

AtomicCounterBuffer& AtomicCounterBuffer::operator=(
    AtomicCounterBuffer&& other) {
  parent_t::operator=(std::move(other));
  return *this;
}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(size_t n, usage_t usage)
    : Buffer(n, usage) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(size_t n, unsigned int initial,
                                         usage_t usage)
    : Buffer(n, initial, usage) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(const std::vector<unsigned int>& data,
                                         usage_t                          usage)
    : Buffer(data, usage) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(
    std::initializer_list<unsigned int>&& list)
    : Buffer(std::move(list), default_usage) {}

//------------------------------------------------------------------------------

void AtomicCounterBuffer::set_all_to(unsigned int val) {
  auto gpu_data = reinterpret_cast<unsigned char*>(
      glMapNamedBuffer(this->m_gl_handle, GL_WRITE_ONLY));
  gl_error_check("glMapNamedBuffer");
  std::memset(gpu_data, val, sizeof(unsigned int) * size());
  glUnmapNamedBuffer(this->m_gl_handle);
}

//------------------------------------------------------------------------------

void AtomicCounterBuffer::bind(size_t i) {
  glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, i, this->m_gl_handle);
}

//==============================================================================
}  // namespace Yavin
//==============================================================================
