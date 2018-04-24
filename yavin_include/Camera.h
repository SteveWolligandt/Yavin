#ifndef __YAVIN_CAMERA_H__
#define __YAVIN_CAMERA_H__

#include <glm/glm.hpp>
#include "Movable.h"
#include "Transform.h"
#include "gl_functions.h"

//==============================================================================
namespace Yavin {
//==============================================================================

class Camera : public Movable {
 public:
  Camera(const float vp_x, const float vp_y, const float vp_width,
         const float vp_height)
      : m_viewport{vp_x, vp_y, vp_width, vp_height} {}
  Camera(const float vp_width, const float vp_height)
      : m_viewport{0, 0, vp_width, vp_height} {}
  Camera(const glm::vec4& vp) : m_viewport(vp) {}
  Camera(glm::vec4&& vp) : m_viewport(std::move(vp)) {}

  auto&       projection_matrix() { return m_projection_matrix; }
  const auto& projection_matrix() const { return m_projection_matrix; }
  glm::mat4x4 view_matrix() const { return glm::inverse(m_transform.matrix()); }

  void set_viewport(const float vp_x, const float vp_y, const float vp_width,
                    const float vp_height) {
    m_viewport = {vp_x, vp_y, vp_width, vp_height};
  }
  void set_viewport(const float vp_width, const float vp_height) {
    m_viewport = {0, 0, vp_width, vp_height};
  }
  void set_viewport(const glm::vec4& vp) { m_viewport = vp; }
  void set_viewport(glm::vec4&& vp) { m_viewport = std::move(vp); }

  auto&       viewport() { return m_viewport; }
  const auto& viewport() const { return m_viewport; }

 protected:
  glm::mat4x4 m_projection_matrix;
  glm::vec4   m_viewport;
};

// namespace gl {
// inline void viewport(const Camera& cam) {
//   viewport(cam.viewport().x, cam.viewport().y, cam.viewport().z,
//            cam.viewport().w);
// }
// //==============================================================================
// }  // namespace gl
// //==============================================================================

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
