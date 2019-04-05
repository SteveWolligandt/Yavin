#ifndef __YAVIN_ID_HOLDER_H__
#define __YAVIN_ID_HOLDER_H__

#include <utility>
#include "gl_includes.h"

//==============================================================================
namespace yavin {
//==============================================================================
template <typename id_t>
struct id_holder_default_param;

template <>
struct id_holder_default_param<GLuint> {
  constexpr static GLuint value = 0;
};

template <>
struct id_holder_default_param<GLint> {
  constexpr static GLuint value = -1;
};

template <typename id_t>
static constexpr auto id_holder_default_param_v =
    id_holder_default_param<id_t>::value;

template <typename id_t>
struct id_holder {
  static constexpr auto default_val = id_holder_default_param_v<id_t>;
  id_holder() : m_id{default_val} {}
  id_holder(id_t _id) : m_id{_id} {}
  id_holder(id_holder&& other) : m_id{std::exchange(other.m_id, default_val)} {}
  auto& operator=(id_holder&& other) {
    swap(other);
    return *this;
  }
  auto& operator=(const id_holder& other) = delete;
  id_holder(const id_holder& other)       = delete;

  auto  id() const { return m_id; }
  auto& id() { return m_id; }

  void swap(id_holder& other) { std::swap(m_id, other.m_id); }

 protected:
  id_t m_id;
};

template <typename id_t>
void swap(id_holder<id_t>& i0, id_holder<id_t>& i1) {
  i0.swap(i1);
}

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
