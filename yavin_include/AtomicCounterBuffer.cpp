#include "AtomicCounterBuffer.h"

#include <cstring>
#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

AtomicCounterBuffer::AtomicCounterBuffer(parent_t::usage_t usage)
    : Buffer(usage) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(const AtomicCounterBuffer& other)
    : Buffer(other) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(AtomicCounterBuffer&& other)
    : Buffer(std::move(other)) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(const std::vector<unsigned int>& data,
                                         parent_t::usage_t                usage,
                                         bool direct_upload,
                                         bool keep_data_on_cpu)
    : Buffer(data, usage, direct_upload, keep_data_on_cpu) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(std::vector<unsigned int>&& data,
                                         parent_t::usage_t           usage,
                                         bool direct_upload,
                                         bool keep_data_on_cpu)
    : Buffer(std::move(data), usage, direct_upload, keep_data_on_cpu) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(
    std::initializer_list<unsigned int>&& list)
    : Buffer(std::move(list), parent_t::DYNAMIC_DRAW) {}

//------------------------------------------------------------------------------

void AtomicCounterBuffer::set_all_to(unsigned int val) {
  auto gpu_data = reinterpret_cast<unsigned char*>(
      glMapNamedBuffer(this->m_id, GL_WRITE_ONLY));
  gl_error_check("glMapNamedBuffer");
  std::memset(gpu_data, val, sizeof(unsigned int) * m_gpu_size);
  // for (size_t i = 0; i < m_gpu_size; ++i) gpu_data[i] = val;
  glUnmapNamedBuffer(this->m_id);
}

void AtomicCounterBuffer::bind(size_t i) {
  glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, i, this->m_id);
}
//==============================================================================
}  // namespace Yavin
//==============================================================================
