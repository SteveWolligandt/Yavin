#ifndef __YAVIN_ATOMIC_COUNTER_BUFFER_H__
#define __YAVIN_ATOMIC_COUNTER_BUFFER_H__

#include <initializer_list>
#include <iostream>
#include <vector>
#include "Buffer.h"
#include "dll_export.h"
#include "error_check.h"

//==============================================================================
namespace Yavin {
//==============================================================================

class AtomicCounterBuffer
    : public Buffer<GL_ATOMIC_COUNTER_BUFFER, unsigned int> {
 public:
  using parent_t = Buffer<GL_ATOMIC_COUNTER_BUFFER, unsigned int>;
  using this_t   = AtomicCounterBuffer;
  static const usage_t default_usage = DYNAMIC_DRAW;

  DLL_API AtomicCounterBuffer(usage_t usage = default_usage);
  DLL_API AtomicCounterBuffer(const this_t& other);
  DLL_API AtomicCounterBuffer(this_t&& other);
  DLL_API this_t& operator=(const this_t& other);
  DLL_API this_t& operator=(this_t&& other);

  DLL_API AtomicCounterBuffer(size_t n, usage_t usage = default_usage);
  DLL_API AtomicCounterBuffer(size_t n, unsigned int initial,
                              usage_t usage = default_usage);
  DLL_API AtomicCounterBuffer(const std::vector<unsigned int>& data,
                              usage_t usage = default_usage);
  DLL_API AtomicCounterBuffer(std::initializer_list<unsigned int>&& data);

  DLL_API void set_all_to(unsigned int val);
  void         to_zero() { set_all_to(0); }
  DLL_API void bind(size_t i);
};

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
