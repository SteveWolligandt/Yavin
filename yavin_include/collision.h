#ifndef __COLLISION_H__
#define __COLLISION_H__

#include <glm/glm.hpp>
#include "scene_object.h"
namespace Yavin {

template <typename T>
class collision_t {
 public:
  collision_t(scene_object_t<T> _o, const glm::tvec3<T>& _x) : m_scene_object(&_o), m_x(_x) {}
  collision_t(scene_object_t<T> _o, glm::tvec3<T>&& _x) : m_scene_object(&_o), m_x(std::move(_x)) {}
  collision_t(scene_object_t<T>* _o, const glm::tvec3<T>& _x) : m_scene_object(_o), m_x(_x) {}
  collision_t(scene_object_t<T>* _o, glm::tvec3<T>&& _x) : m_scene_object(_o), m_x(std::move(_x)) {}

  collision_t(const collision_t<T>& other) : m_scene_object(other.m_scene_object), m_x(other.m_x) {}
  collision_t(collision_t<T>&& other) : m_scene_object(other.m_scene_object), m_x(std::move(other.m_x)) {}

  auto& operator=(const collision_t<T>& other) {
    m_scene_object = other.m_scene_object;
    m_x            = other.m_x;
    return *this;
  }

  auto& operator=(collision_t<T>&& other) {
    m_scene_object = other.m_scene_object;
    m_x            = std::move(other.m_x);
    return *this;
  }

  auto&       x() { return m_x; }
  const auto& x() const { return m_x; }
  const auto& x(size_t i) { return m_x[i]; }
  auto&       object() { return *m_scene_object; }
  const auto& object() const { return *m_scene_object; }

 private:
  scene_object_t<T>* m_scene_object;
  glm::tvec3<T>      m_x;
};

using collision_d = collision_t<double>;
using collision_f = collision_t<float>;

using collision = collision_t<double>;
}  // namespace Yavin

#endif