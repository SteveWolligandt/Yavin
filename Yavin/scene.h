#ifndef __SCENE_H__
#define __SCENE_H__

#include <algorithm>
#include <iostream>
#include <vector>
#include "Camera.h"
#include "collision.h"
#include "ray.h"
#include "scene_object.h"

namespace Yavin {

template <typename T = double>
class scene_t : public std::vector<scene_object_t<T>> {
 public:
  std::vector<std::unique_ptr<Shader>> m_shader_pool;

  template <typename shader_t, typename... Args>
  auto& add_shader(Args&&... args) {
    m_shader_pool.emplace_back(new shader_t(std::forward<Args>(args)...));
    return *dynamic_cast<shader_t*>(m_shader_pool.back().get());
  }

  std::optional<collision_t<T>> cast_ray(const Yavin::ray_t<T>& r) const {
    T                             shortest_dist = 1e10;
    std::optional<collision_t<T>> closest_collision;
    for (const auto& obj : *this)
      if (auto c = obj.check_collision(r); c)
        if (T dist = glm::distance(c->x(), r.x()); shortest_dist > dist) {
          shortest_dist     = dist;
          closest_collision = c;
        }

    if (closest_collision) closest_collision->object().on_collision(*closest_collision);

    return closest_collision;
  }

  void push_back(const scene_object_t<T>& obj) {
    std::vector<scene_object_t<T>>::push_back(obj);
    obj.m_appropriate_scenes.push_back(this);
  }

  void push_back(scene_object_t<T>&& obj) {
    std::vector<scene_object_t<T>>::push_back(std::move(obj));
    obj.m_appropriate_scenes.push_back(this);
  }

  template <class... Args>
  auto& emplace_back(Args&&... args) {
    std::vector<scene_object_t<T>>::emplace_back(std::forward<Args>(args)...);
    this->back().m_appropriate_scenes.push_back(this);
    return this->back();
  }

  std::optional<collision_t<T>> cast_ray(const Yavin::ray_t<T>& r, T x, T y) const {
    auto c = cast_ray(r);
    if (c) c->object().on_mouse_down(x, y);
    return c;
  }

  void on_mouse_down(T x, T y) {
    for (auto& obj : *this) obj.on_mouse_down(x, y);
  }

  void on_mouse_up(T x, T y) {
    for (auto& obj : *this) obj.on_mouse_up(x, y);
  }

  void on_mouse_moved(T x, T y) {
    for (auto& obj : *this) obj.on_mouse_moved(x, y);
  }

  void on_collision(const collision_t<T>& c) {
    for (auto& obj : *this) obj.on_collision(c);
  }

  void on_render(const Yavin::Camera& cam) {
    Yavin::set_viewport(cam.viewport());
    for (auto& obj : *this) obj.on_render(cam);
  }
};

using scene_d = scene_t<double>;
using scene_f = scene_t<float>;

using scene = scene_t<double>;

}  // namespace Yavin
#endif