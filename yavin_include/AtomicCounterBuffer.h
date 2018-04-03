#ifndef __YAVIN_ATOMIC_COUNTER_BUFFER_H__
#define __YAVIN_ATOMIC_COUNTER_BUFFER_H__

#include <initializer_list>
#include <iostream>
#include <vector>
#include "Buffer.h"
#include "dll_export.h"
#include "error_check.h"

namespace Yavin {
class AtomicCounterBuffer
    : public Buffer<GL_ATOMIC_COUNTER_BUFFER, unsigned int> {
 public:
  DLL_API AtomicCounterBuffer();
  DLL_API AtomicCounterBuffer(const AtomicCounterBuffer& other);
  DLL_API AtomicCounterBuffer(AtomicCounterBuffer&& other);
  DLL_API AtomicCounterBuffer(const std::vector<unsigned int>& data,
                              bool direct_upload    = false,
                              bool keep_data_on_cpu = false);
  DLL_API AtomicCounterBuffer(std::vector<unsigned int>&& data,
                              bool                        direct_upload = false,
                              bool keep_data_on_cpu = false);
  DLL_API AtomicCounterBuffer(std::initializer_list<unsigned int>&&);

  DLL_API void set_all_to(unsigned int val);
  void         to_zero() { set_all_to(0); }
  DLL_API void bind(size_t i);
};
}  // namespace Yavin

#endif
