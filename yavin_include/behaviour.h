#ifndef __YAVIN_BEHAVIOUR_H__
#define __YAVIN_BEHAVIOUR_H__

#include <optional>
#include "camera.h"
#include "ray.h"

//==============================================================================
namespace yavin {
//==============================================================================

template <typename T>
class scene_object_t;
template <typename T>
class collision_t;

template <typename T>
class behaviour_t {
 public:
  behaviour_t(scene_object_t<T>* o) : m_scene_object(o) {}
  behaviour_t(const behaviour_t<T>& other)
      : m_scene_object(other.m_scene_object) {}
  behaviour_t(behaviour_t<T>&& other) : m_scene_object(other.m_scene_object) {}

  virtual void on_mouse_down(T /*x*/, T /*y*/) {}
  virtual void on_mouse_up(T /*x*/, T /*y*/) {}
  virtual void on_mouse_moved(T /*x*/, T /*y*/) {}
  virtual void on_collision(const collision_t<T>& /*c*/) {}
  virtual void on_render(const Camera& /*cam*/) {}

  virtual std::optional<collision_t<T>> check_collision(
      const ray_t<T>& /*r*/) {
    return {};
  }

  virtual behaviour_t<T>& operator=(const behaviour_t& other) {
    m_scene_object = other.m_scene_object;
    return *this;
  }

  virtual behaviour_t<T>& operator=(behaviour_t&& other) {
    m_scene_object = other.m_scene_object;
    return *this;
  }

  auto&       object() { return *m_scene_object; }
  const auto& object() const { return *m_scene_object; }

 protected:
  scene_object_t<T>* m_scene_object;
};

using behaviour_d = behaviour_t<double>;
using behaviour_f = behaviour_t<float>;

using behaviour = behaviour_t<double>;

//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
