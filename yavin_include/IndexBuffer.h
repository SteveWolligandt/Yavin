#ifndef __YAVIN_INDEXBUFFER_H__
#define __YAVIN_INDEXBUFFER_H__

#include <initializer_list>
#include <iostream>
#include <vector>
#include "Buffer.h"
#include "dll_export.h"
#include "error_check.h"

namespace Yavin {
class IndexBuffer : public Buffer<GL_ELEMENT_ARRAY_BUFFER, unsigned int> {
 public:
  using parent_t = Buffer<GL_ELEMENT_ARRAY_BUFFER, unsigned int>;
  static const usage_t default_usage = STATIC_DRAW;

  DLL_API IndexBuffer(usage_t usage = default_usage);
  DLL_API IndexBuffer(const IndexBuffer& other);
  DLL_API IndexBuffer(IndexBuffer&& other);
  DLL_API IndexBuffer(const std::vector<unsigned int>& data,
                      usage_t usage = default_usage, bool direct_upload = true,
                      bool keep_data_on_cpu = false);
  DLL_API IndexBuffer(std::vector<unsigned int>&& data,
                      usage_t usage = default_usage, bool direct_upload = true,
                      bool keep_data_on_cpu = false);
  DLL_API IndexBuffer(std::initializer_list<unsigned int>&& list);
};
}  // namespace Yavin

#endif
