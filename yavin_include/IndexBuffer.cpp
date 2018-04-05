#include "IndexBuffer.h"

#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

IndexBuffer::IndexBuffer(usage_t usage) : Buffer(usage) {}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(const IndexBuffer& other) : Buffer(other) {}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(IndexBuffer&& other) : Buffer(std::move(other)) {}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(const std::vector<unsigned int>& data, usage_t usage,
                         bool direct_upload, bool keep_data_on_cpu)
    : Buffer(data, usage, direct_upload, keep_data_on_cpu) {}

IndexBuffer::IndexBuffer(std::vector<unsigned int>&& data, usage_t usage,
                         bool direct_upload, bool keep_data_on_cpu)
    : Buffer(std::move(data), usage, direct_upload, keep_data_on_cpu) {}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(std::initializer_list<unsigned int>&& list)
    : Buffer(std::move(list), default_usage) {}

//==============================================================================
}  // namespace Yavin
//==============================================================================
