#ifndef __SCENE_OBJECT_H__
#define __SCENE_OBJECT_H__

#include <exception>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <yavin>
#include "camera.h"
#include "movable.h"
#include "behaviour.h"

//==============================================================================
namespace yavin {
//==============================================================================

template <typename T>
class scene_t;

template <typename T>
class collider_t;

template <typename T>
class collision_t;

template <typename T>
struct is_collider;

template <typename T>
class scene_object_t : public std::vector<std::unique_ptr<behaviour_t<T>>>,
                       public Movable {
  friend class scene_t<T>;

 public:
  scene_object_t()
      : m_collider(nullptr), m_parent_object(nullptr), m_name("unnamed") {}
  scene_object_t(std::string&& name)
      : m_collider(nullptr),
        m_parent_object(nullptr),
        m_name(std::move(name)) {}

  scene_object_t(scene_object_t<T>&& other)
      : std::vector<std::unique_ptr<behaviour_t<T>>>(std::move(other)),
        Movable(std::move(other)),
        m_collider(std::move(other.m_collider)),
        m_parent_object(std::move(other.m_parent_object)),
        m_name(std::move(other.m_name)) {}

  template <typename behaviour_t, typename... Args>
  auto& add_behaviour(Args&&... args) {
    if constexpr (is_collider<behaviour_t>::value) {
      if (!m_collider) {
        m_collider =
            std::make_unique<behaviour_t>(this, std::forward<Args>(args)...);
        return *dynamic_cast<behaviour_t*>(m_collider.get());
      } else
        throw std::runtime_error("scene object already has collider");

    } else {
      this->emplace_back(new behaviour_t(this, std::forward<Args>(args)...));
      return *dynamic_cast<behaviour_t*>(this->back().get());
    }
  }

  std::optional<collision_t<T>> check_collision(const ray_t<T>& r) const {
    if (m_collider)
      return m_collider->check_collision(r);
    else
      return {};
  }

  void on_mouse_down(T x, T y) {
    for (const auto& b : *this) b->on_mouse_down(x, y);
  }

  void on_mouse_up(T x, T y) {
    for (const auto& b : *this) b->on_mouse_up(x, y);
  }

  void on_mouse_moved(T x, T y) {
    for (const auto& b : *this) b->on_mouse_moved(x, y);
  }

  void on_collision(const collision_t<T>& c) {
    for (const auto& b : *this) b->on_collision(c);
  }

  void on_render(const Camera& cam) {
    for (auto& b : *this) b->on_render(cam);
  }

  template <typename collider_cast_t>
  const auto& collider() const {
    return *static_cast<collider_cast_t*>(m_collider.get());
  }
  template <typename collider_cast_t>
  auto& collider() {
    return *static_cast<collider_cast_t*>(m_collider.get());
  }

  auto&       parent() { return *m_parent_object; }
  const auto& parent() const { return *m_parent_object; }
  void        set_parent(scene_object_t<T>& o) { m_parent_object = &o; }
  void        unset_parent() { m_parent_object = nullptr; }

  glm::mat4x4 transformation_matrix() const {
    if (m_parent_object)
      return transform().matrix() * m_parent_object->transformation_matrix();
    else
      return transform().matrix();
  }

  auto&       name() { return m_name; }
  const auto& name() const { return m_name; }

  auto&       appropriate_scenes() { return m_appropriate_scenes; }
  const auto& appropriate_scenes() const { return m_appropriate_scenes; }

 protected:
  std::unique_ptr<collider_t<T>> m_collider;
  scene_object_t<T>*             m_parent_object;
  std::string                    m_name;
  std::vector<scene_t<T>*>
      m_appropriate_scenes;  // friend class scene_t adds itself on push_back
};

using scene_object_d = scene_object_t<double>;
using scene_object_f = scene_object_t<float>;

using scene_object = scene_object_t<double>;

//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
