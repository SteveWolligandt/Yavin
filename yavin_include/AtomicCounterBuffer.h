#ifndef __YAVIN_ATOMIC_COUNTER_BUFFER_H__
#define __YAVIN_ATOMIC_COUNTER_BUFFER_H__

#include <initializer_list>
#include <iostream>
#include <vector>
#include "Buffer.h"
#include "error_check.h"
#include "dll_export.h"

namespace Yavin {
class AtomicCounterBuffer
    : public Buffer<GL_ATOMIC_COUNTER_BUFFER, unsigned int> {
 public:
  DLL_API AtomicCounterBuffer();
  DLL_API AtomicCounterBuffer(const AtomicCounterBuffer& other);
  DLL_API AtomicCounterBuffer(AtomicCounterBuffer&& other);
  DLL_API AtomicCounterBuffer(const std::vector<unsigned int>& data,
                      bool                             direct_upload = false,
                      bool keep_data_on_cpu                          = false);
  DLL_API AtomicCounterBuffer(std::vector<unsigned int>&& data,
                      bool                        direct_upload    = false,
                      bool                        keep_data_on_cpu = false);
  DLL_API AtomicCounterBuffer(std::initializer_list<unsigned int>&& list,
                      bool direct_upload    = false,
                      bool keep_data_on_cpu = false);

  DLL_API void set_all_to(unsigned int val);
  DLL_API void to_zero() { set_all_to(0); }
};
}  // namespace Yavin

#endif
