#ifndef __YAVIN_PERSPECTIVE_CAMERA_H__
#define __YAVIN_PERSPECTIVE_CAMERA_H__

#include <glm/glm.hpp>
#include "Camera.h"
#include "dll_export.h"
#include "ray.h"

//==============================================================================
namespace Yavin {
//==============================================================================

class PerspectiveCamera : public Camera {
 public:
  DLL_API PerspectiveCamera(float fovy, float aspect, float near, float far,
                            const float vp_x, float vp_y, float vp_width,
                            float vp_height);

  DLL_API PerspectiveCamera(float fovy, float aspect, float near, float far,
                            float vp_width, float vp_height);

  DLL_API PerspectiveCamera(float fovy, float aspect, float near, float far,
                            glm::vec4& vp);

  DLL_API PerspectiveCamera(float fovy, float aspect, float near, float far,
                            glm::vec4&& vp);

  DLL_API void set_projection(float fovy, float aspect, float near, float far);
  DLL_API void set_projection(float fovy, float aspect, float near, float far,
                              float vp_width, float vp_height);
  DLL_API void set_projection(float fovy, float aspect, float near, float far,
                              float vp_x, float vp_y, float vp_width,
                              float vp_height);

  template <typename T>
  auto create_ray(T x, T y) {
    glm::vec3 screen_coord{x, y, 0.5};
    glm::vec3 world_pos = glm::unProject(screen_coord, view_matrix(),
                                         projection_matrix(), m_viewport);
    glm::vec3 origin    = transform().translation();
    return ray_t<T>(origin, glm::normalize(world_pos - origin));
  }
};

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
