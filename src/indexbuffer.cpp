#include <yavin/indexbuffer.h>

#include <yavin/glincludes.h>

//==============================================================================
namespace yavin {
//==============================================================================

IndexBuffer::IndexBuffer(usage_t usage) : Buffer(usage) {}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(const IndexBuffer& other) : Buffer(other) {}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(IndexBuffer&& other) : Buffer(std::move(other)) {}

//------------------------------------------------------------------------------

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& other) {
  parent_t::operator=(other);
  return *this;
}

//------------------------------------------------------------------------------

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) {
  parent_t::operator=(std::move(other));
  return *this;
}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(size_t n, usage_t usage) : Buffer(n, usage) {}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(size_t n, unsigned int initial, usage_t usage)
    : Buffer(n, initial, usage) {}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(const std::vector<unsigned int>& data, usage_t usage)
    : Buffer(data, usage) {}

//------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(std::initializer_list<unsigned int>&& list)
    : Buffer(std::move(list), default_usage) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
