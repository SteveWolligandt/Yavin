#include "IndexBuffer.h"

#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

IndexBuffer::IndexBuffer() : Buffer() {}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(const IndexBuffer& other) : Buffer(other) {}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(IndexBuffer&& other) : Buffer(std::move(other)) {}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(const std::vector<unsigned int>& data,
                         bool direct_upload, bool keep_data_on_cpu)
    : Buffer(data, direct_upload, keep_data_on_cpu) {}

IndexBuffer::IndexBuffer(std::vector<unsigned int>&& data, bool direct_upload,
                         bool keep_data_on_cpu)
    : Buffer(std::move(data), direct_upload, keep_data_on_cpu) {}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(std::initializer_list<unsigned int>&& list)
    : Buffer(std::move(list)) {}

//==============================================================================
}  // namespace Yavin
//==============================================================================
