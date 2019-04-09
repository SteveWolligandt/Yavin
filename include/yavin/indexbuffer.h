#ifndef __YAVIN_INDEXBUFFER_H__
#define __YAVIN_INDEXBUFFER_H__

#include <initializer_list>
#include <iostream>
#include <vector>
#include "buffer.h"
#include "dllexport.h"
#include "errorcheck.h"

//==============================================================================
namespace yavin {
//==============================================================================

class IndexBuffer : public Buffer<GL_ELEMENT_ARRAY_BUFFER, unsigned int> {
 public:
  using parent_t = Buffer<GL_ELEMENT_ARRAY_BUFFER, unsigned int>;
  using this_t   = IndexBuffer;

  static const usage_t default_usage = STATIC_DRAW;

  DLL_API IndexBuffer(usage_t usage = default_usage);
  DLL_API IndexBuffer(const IndexBuffer& other);
  DLL_API IndexBuffer(IndexBuffer&& other);
  DLL_API this_t& operator=(const this_t& other);
  DLL_API this_t& operator=(this_t&& other);

  DLL_API IndexBuffer(size_t n, usage_t usage = default_usage);
  DLL_API IndexBuffer(size_t n, unsigned int initial,
                      usage_t usage = default_usage);
  DLL_API IndexBuffer(const std::vector<unsigned int>& data,
                      usage_t                          usage = default_usage);
  DLL_API IndexBuffer(std::initializer_list<unsigned int>&& list);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
