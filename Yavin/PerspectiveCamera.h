#ifndef __YAVIN_PERSPECTIVE_CAMERA_H__
#define __YAVIN_PERSPECTIVE_CAMERA_H__

#include <glm/glm.hpp>
#include "Camera.h"
#include "ray.h"

namespace Yavin {
class PerspectiveCamera : public Camera {
 public:
  PerspectiveCamera(const float fovy, const float aspect, const float near, const float far, const float vp_x,
                    const float vp_y, const float vp_width, const float vp_height);

  PerspectiveCamera(const float fovy, const float aspect, const float near, const float far, const float vp_width,
                    const float vp_height);

  PerspectiveCamera(const float fovy, const float aspect, const float near, const float far, const glm::vec4& vp);

  PerspectiveCamera(const float fovy, const float aspect, const float near, const float far, glm::vec4&& vp);

  void set_projection(const float fovy, const float aspect, const float near, const float far);

  template <typename T>
  auto create_ray(T x, T y) {
    glm::vec3 screen_coord{x, y, 0.5};
    glm::vec3 world_pos = glm::unProject(screen_coord, view_matrix(), projection_matrix(), m_viewport);
    glm::vec3 origin    = transform().translation();
    return ray_t<T>(origin, glm::normalize(world_pos - origin));
  }
};
}  // namespace Yavin

#endif