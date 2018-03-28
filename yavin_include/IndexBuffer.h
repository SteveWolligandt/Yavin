#ifndef __YAVIN_INDEXBUFFER_H__
#define __YAVIN_INDEXBUFFER_H__

#include <initializer_list>
#include <iostream>
#include <vector>
#include "Buffer.h"
#include "error_check.h"
#include "dll_export.h"

namespace Yavin {
class IndexBuffer : public Buffer<GL_ELEMENT_ARRAY_BUFFER, unsigned int> {
 public:
  DLL_API IndexBuffer();
  DLL_API IndexBuffer(const IndexBuffer& other);
  DLL_API IndexBuffer(IndexBuffer&& other);
  DLL_API IndexBuffer(const std::vector<unsigned int>& data, bool direct_upload = false,
              bool keep_data_on_cpu = false);
  DLL_API IndexBuffer(std::vector<unsigned int>&& data, bool direct_upload = false,
              bool keep_data_on_cpu = false);
  DLL_API IndexBuffer(std::initializer_list<unsigned int>&& list, bool direct_upload = false,
              bool keep_data_on_cpu = false);
};
}  // namespace Yavin

#endif