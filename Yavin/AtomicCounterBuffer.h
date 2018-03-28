#ifndef __YAVIN_ATOMIC_COUNTER_BUFFER_H__
#define __YAVIN_ATOMIC_COUNTER_BUFFER_H__

#include <initializer_list>
#include <iostream>
#include <vector>
#include "Buffer.h"
#include "error_check.h"

namespace Yavin {
class IndexBuffer : public Buffer<GL_ATOMIC_COUNTER_BUFFER, unsigned int> {
 public:
  AtomicCounterBuffer();
  AtomicCounterBuffer(const IndexBuffer& other);
  AtomicCounterBuffer(IndexBuffer&& other);
  AtomicCounterBuffer(const std::vector<unsigned int>& data,
                      bool                             direct_upload = false,
                      bool keep_data_on_cpu                          = false);
  AtomicCounterBuffer(std::vector<unsigned int>&& data,
                      bool                        direct_upload    = false,
                      bool                        keep_data_on_cpu = false);
  AtomicCounterBuffer(std::initializer_list<unsigned int>&& list,
                      bool direct_upload    = false,
                      bool keep_data_on_cpu = false);

  void set_all_to(unsigned int val);
  void to_zero() { set_all_to(0); }
};
}  // namespace Yavin

#endif
