#include "AtomicCounterBuffer.h"

#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

AtomicCounterBuffer::AtomicCounterBuffer() : Buffer() {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(const AtomicCounterBuffer& other)
    : Buffer(other) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(AtomicCounterBuffer&& other)
    : Buffer(std::move(other)) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(const std::vector<unsigned int>& data,
                                         bool direct_upload,
                                         bool keep_data_on_cpu)
    : Buffer(data, direct_upload, keep_data_on_cpu) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(std::vector<unsigned int>&& data,
                                         bool direct_upload,
                                         bool keep_data_on_cpu)
    : Buffer(std::move(data), direct_upload, keep_data_on_cpu) {}

//------------------------------------------------------------------------------

AtomicCounterBuffer::AtomicCounterBuffer(
    std::initializer_list<unsigned int>&& list, bool direct_upload,
    bool keep_data_on_cpu)
    : Buffer(std::move(list), direct_upload, keep_data_on_cpu) {}

//------------------------------------------------------------------------------

void AtomicCounterBuffer::set_all_to(unsigned int val) {
  auto userCounters = reinterpret_cast<unsigned char*>(
      glMapNamedBuffer(this->m_id, GL_WRITE_ONLY));
  gl_error_check("glMapNamedBuffer");
  memset(userCounters, val, sizeof(unsigned int) * m_gpu_size);
  glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
}
//==============================================================================
}  // namespace Yavin
//==============================================================================
