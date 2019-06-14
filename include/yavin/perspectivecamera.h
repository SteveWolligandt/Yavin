#ifndef __YAVIN_PERSPECTIVE_CAMERA_H__
#define __YAVIN_PERSPECTIVE_CAMERA_H__

#include <glm/glm.hpp>
#include "camera.h"
#include "dllexport.h"
#include "ray.h"

//==============================================================================
namespace yavin {
//==============================================================================

class perspectivecamera : public camera {
 public:
  DLL_API perspectivecamera(float fovy, float aspect, float near, float far,
                            size_t vp_x, size_t vp_y, size_t vp_width,
                            size_t vp_height);

  DLL_API perspectivecamera(float fovy, float aspect, float near, float far,
                            size_t vp_width, size_t vp_height);

  DLL_API perspectivecamera(float fovy, float aspect, float near, float far,
                            glm::uvec4& vp);

  DLL_API perspectivecamera(float fovy, float aspect, float near, float far,
                            glm::uvec4&& vp);

  DLL_API void set_projection(float fovy, float aspect, float near, float far);
  DLL_API void set_projection(float fovy, float aspect, float near, float far,
                              size_t vp_width, size_t vp_height);
  DLL_API void set_projection(float fovy, float aspect, float near, float far,
                              size_t vp_x, size_t vp_y, size_t vp_width,
                              size_t vp_height);

  template <typename T>
  auto create_ray(T x, T y) {
    glm::vec3 screen_coord{x, y, 0.5};
    glm::vec3 world_pos = glm::unProject(screen_coord, view_matrix(),
                                         projection_matrix(), m_viewport);
    glm::vec3 origin    = transform().translation();
    return ray<T>(origin, glm::normalize(world_pos - origin));
  }
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
