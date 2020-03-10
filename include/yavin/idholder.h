#ifndef YAVIN_ID_HOLDER_H
#define YAVIN_ID_HOLDER_H

#include <utility>
#include "glincludes.h"

//==============================================================================
namespace yavin {
//==============================================================================
template <typename id_t>
struct id_holder_default_param;

//==============================================================================
template <>
struct id_holder_default_param<GLuint> {
  constexpr static GLuint value = 0;
};

//==============================================================================
template <>
struct id_holder_default_param<GLint> {
  constexpr static GLuint value = -1;
};

//==============================================================================
template <typename id_t>
static constexpr auto id_holder_default_param_v =
    id_holder_default_param<id_t>::value;

//==============================================================================
template <typename id_t>
struct id_holder {
  static constexpr auto default_val = id_holder_default_param_v<id_t>;

 private:
  //----------------------------------------------------------------------------
  id_t m_id;

 public:
  //----------------------------------------------------------------------------
  DLL_API id_holder() : m_id{default_val} {}
  //----------------------------------------------------------------------------
  DLL_API id_holder(id_t _id) : m_id{_id} {}
  //----------------------------------------------------------------------------
  DLL_API id_holder(id_holder&& other)
      : m_id{std::exchange(other.m_id, default_val)} {}
  //----------------------------------------------------------------------------
  DLL_API auto& operator=(id_holder&& other) {
    swap(other);
    return *this;
  }
  //----------------------------------------------------------------------------
  DLL_API auto& operator=(const id_holder& other) = delete;
  //----------------------------------------------------------------------------
  DLL_API id_holder(const id_holder& other) = delete;
  //----------------------------------------------------------------------------
  DLL_API auto id() const { return m_id; }
  //----------------------------------------------------------------------------
 protected:
  DLL_API void set_id(id_t id) { m_id = id; }
  //----------------------------------------------------------------------------
  DLL_API auto& id_ref() { return m_id; }
  //----------------------------------------------------------------------------
 public:
  DLL_API void swap(id_holder& other) { std::swap(m_id, other.m_id); }
};

//==============================================================================
template <typename id_t>
DLL_API void swap(id_holder<id_t>& i0, id_holder<id_t>& i1) {
  i0.swap(i1);
}

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
