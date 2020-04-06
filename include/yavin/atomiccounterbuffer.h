#ifndef YAVIN_ATOMIC_COUNTER_BUFFER_H
#define YAVIN_ATOMIC_COUNTER_BUFFER_H
//==============================================================================
#include <initializer_list>
#include <iostream>
#include <vector>
#include "buffer.h"
#include "dllexport.h"
#include "errorcheck.h"
//==============================================================================
namespace yavin {
//==============================================================================
class atomiccounterbuffer
    : public buffer<GL_ATOMIC_COUNTER_BUFFER, unsigned int> {
 public:
  using parent_t = buffer<GL_ATOMIC_COUNTER_BUFFER, unsigned int>;
  using this_t   = atomiccounterbuffer;
  static const usage_t default_usage = DYNAMIC_DRAW;

  DLL_API explicit atomiccounterbuffer(usage_t usage = default_usage);
  DLL_API atomiccounterbuffer(const this_t& other);
  DLL_API atomiccounterbuffer(this_t&& other) noexcept;
  DLL_API auto operator=(const this_t& other) -> atomiccounterbuffer&;
  DLL_API auto operator=(this_t&& other) noexcept -> atomiccounterbuffer&;
  ~atomiccounterbuffer() = default;

  DLL_API atomiccounterbuffer(size_t n, usage_t usage = default_usage);
  DLL_API atomiccounterbuffer(size_t n, unsigned int initial,
                              usage_t usage = default_usage);
  DLL_API atomiccounterbuffer(const std::vector<unsigned int>& data,
                              usage_t usage = default_usage);
  DLL_API atomiccounterbuffer(std::initializer_list<unsigned int>&& data);

  DLL_API void        set_all_to(unsigned int val);
  void                to_zero() { set_all_to(0); }
  DLL_API void        bind(size_t i) const;
  DLL_API static void unbind(size_t i);
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
