#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include <glm/glm.hpp>
#include "behaviour.h"
#include "collision.h"

namespace Yavin {

template <typename T>
class collider_t : public behaviour_t<T> {
 public:
  collider_t(scene_object_t<T>* o) : behaviour_t<T>(o) {}
  collider_t(const collider_t& other) : behaviour_t<T>(other) {}
  collider_t(collider_t&& other) : behaviour_t<T>(other) {}

  virtual collider_t<T>& operator=(const collider_t& other) {
    behaviour_t<T>::operator=(other);
    return *this;
  }

  virtual collider_t<T>& operator=(collider_t&& other) {
    behaviour_t<T>::operator=(other);
    return *this;
  }
};

template <typename T>
class aabb_collider : public collider_t<T> {
 public:
  aabb_collider(scene_object_t<T>* o, float center_x, float center_y, float center_z, float size_x, float size_y,
                float size_z)
      : collider_t<T>(o), m_center{center_x, center_y, center_z}, m_size{size_x, size_y, size_z} {}
  aabb_collider(const aabb_collider& other) : collider_t<T>(other), m_center(other.m_center), m_size(other.m_size) {}
  aabb_collider(aabb_collider&& other)
      : collider_t<T>(other), m_center(std::move(other.m_center)), m_size(std::move(other.m_size)) {}

  virtual aabb_collider<T>& operator=(const aabb_collider& other) {
    collider_t<T>::operator=(other);
    m_center               = other.m_center;
    m_size                 = other.m_size;
    return *this;
  }

  virtual aabb_collider<T>& operator=(aabb_collider&& other) {
    collider_t<T>::operator=(other);
    m_center               = std::move(other.m_center);
    m_size                 = std::move(other.m_size);
    return *this;
  }

  const auto& size() const { return m_size; }
  const auto& size(size_t i) const { return m_size[i]; }

  auto&       center() { return m_center; }
  const auto& center() const { return m_center; }
  const auto& center(size_t i) const { return m_center[i]; }

  auto left() const { return (this->object().transform().matrix() * glm::vec4(m_center, 1))[0] - m_size[0] * 0.5; }
  auto right() const { return (this->object().transform().matrix() * glm::vec4(m_center, 1))[0] + m_size[0] * 0.5; }

  auto front() const { return (this->object().transform().matrix() * glm::vec4(m_center, 1))[1] - m_size[1] * 0.5; }
  auto back() const { return (this->object().transform().matrix() * glm::vec4(m_center, 1))[1] + m_size[1] * 0.5; }

  auto bottom() const { return (this->object().transform().matrix() * glm::vec4(m_center, 1))[2] - m_size[2] * 0.5; }
  auto top() const { return (this->object().transform().matrix() * glm::vec4(m_center, 1))[2] + m_size[2] * 0.5; }

  virtual std::optional<collision_t<T>> check_collision(const Yavin::ray_t<T>& r) override {
    float shortest_dist = 1e10;
    float le            = left();
    float ri            = right();
    float bo            = bottom();
    float to            = top();
    float fr            = front();
    float ba            = back();

    float         left_dist = (le - r.x(0)) / r.dir(0);
    glm::tvec3<T> x_left    = r(left_dist);
    bool          left_hit  = x_left[1] >= fr && x_left[1] <= ba && x_left[2] >= bo && x_left[2] <= to;

    float         right_dist = (ri - r.x(0)) / r.dir(0);
    glm::tvec3<T> x_right    = r(right_dist);
    bool          right_hit  = x_right[1] >= fr && x_right[1] <= ba && x_right[2] >= bo && x_right[2] <= to;

    float         bottom_dist = (bo - r.x(1)) / r.dir(1);
    glm::tvec3<T> x_bottom    = r(bottom_dist);
    bool          bottom_hit  = x_bottom[0] >= le && x_bottom[0] <= ri && x_bottom[2] >= fr && x_bottom[2] <= ba;

    float         top_dist = (to - r.x(1)) / r.dir(1);
    glm::tvec3<T> x_top    = r(top_dist);
    bool          top_hit  = x_top[0] >= le && x_top[0] <= ri && x_top[2] >= fr && x_top[2] <= ba;

    float         front_dist = (fr - r.x(2)) / r.dir(2);
    glm::tvec3<T> x_front    = r(front_dist);
    bool          front_hit  = x_front[0] >= le && x_front[0] <= ri && x_front[1] >= bo && x_front[1] <= to;

    float         back_dist = (ba - r.x(2)) / r.dir(2);
    glm::tvec3<T> x_back    = r(back_dist);
    bool          back_hit  = x_back[0] >= le && x_back[0] <= ri && x_back[1] >= bo && x_back[1] <= to;
    if (left_hit || right_hit || bottom_hit || top_hit || front_hit || back_hit) {
      if (left_hit && shortest_dist > left_dist) shortest_dist = left_dist;
      if (right_hit && shortest_dist > right_dist) shortest_dist = right_dist;
      if (bottom_hit && shortest_dist > bottom_dist) shortest_dist = bottom_dist;
      if (top_hit && shortest_dist > top_dist) shortest_dist = top_dist;
      if (front_hit && shortest_dist > front_dist) shortest_dist = front_dist;
      if (back_hit && shortest_dist > back_dist) shortest_dist = back_dist;
      return collision_t<T>(&this->object(), r(shortest_dist));
    }
    return {};
  }

 private:
  glm::tvec3<T> m_center;
  glm::tvec3<T> m_size;
};

template <typename collider_t>
struct is_collider {
  static constexpr bool value = false;
};

template <typename T>
struct is_collider<collider_t<T>> {
  static constexpr bool value = true;
};

template <typename T>
struct is_collider<aabb_collider<T>> {
  static constexpr bool value = true;
};
}  // namespace Yavin

#endif