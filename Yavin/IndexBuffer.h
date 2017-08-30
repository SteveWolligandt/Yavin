#ifndef __YAVIN_INDEXBUFFER_H__
#define __YAVIN_INDEXBUFFER_H__

#include <initializer_list>
#include <iostream>
#include <vector>
#include "Buffer.h"
#include "error_check.h"

namespace Yavin {
class IndexBuffer : public Buffer<GL_ELEMENT_ARRAY_BUFFER, unsigned int> {
 public:
  IndexBuffer();
  IndexBuffer(const IndexBuffer& other);
  IndexBuffer(IndexBuffer&& other);
  IndexBuffer(const std::vector<unsigned int>& data, bool direct_upload = false,
              bool keep_data_on_cpu = false);
  IndexBuffer(std::vector<unsigned int>&& data, bool direct_upload = false,
              bool keep_data_on_cpu = false);
  IndexBuffer(std::initializer_list<unsigned int>&& list, bool direct_upload = false,
              bool keep_data_on_cpu = false);
};
}  // namespace Yavin

#endif