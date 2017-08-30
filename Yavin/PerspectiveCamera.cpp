#include "PerspectiveCamera.h"

#include "gl_includes.h"

namespace Yavin {
PerspectiveCamera::PerspectiveCamera(const float& fovy, const float& aspect, const float& near,
                                     const float& far) {
  set_projection(fovy, aspect, near, far);
}

void PerspectiveCamera::set_projection(const float& fovy, const float& aspect, const float& near,
                                       const float& far) {
  m_projection_matrix = glm::perspective(fovy, aspect, near, far);
}
}  // Yavin