#ifndef __YAVIN_CAMERA_H__
#define __YAVIN_CAMERA_H__

#include <glm/glm.hpp>
#include "Movable.h"
#include "Transform.h"
#include "gl_includes.h"

namespace Yavin {
class Camera : public Movable {
 public:
  Camera() = default;

  const glm::mat4x4& projection_matrix() { return m_projection_matrix; }
  glm::mat4x4        view_matrix() { return glm::inverse(m_transform.matrix()); }

 protected:
  glm::mat4x4 m_projection_matrix;
};

}  // namespace Yavin

#endif