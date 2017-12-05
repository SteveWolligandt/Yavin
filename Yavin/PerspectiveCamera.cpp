#include "PerspectiveCamera.h"

#include "gl_includes.h"

namespace Yavin {
PerspectiveCamera::PerspectiveCamera(const float fovy, const float aspect, const float near, const float far,
                                     const float vp_x, const float vp_y, const float vp_width, const float vp_height)
    : Camera(vp_x, vp_y, vp_width, vp_height) {
  set_projection(fovy, aspect, near, far);
}

PerspectiveCamera::PerspectiveCamera(const float fovy, const float aspect, const float near, const float far,
                                     const float vp_width, const float vp_height)
    : Camera(vp_width, vp_height) {
  set_projection(fovy, aspect, near, far);
}

PerspectiveCamera::PerspectiveCamera(const float fovy, const float aspect, const float near, const float far,
                                     const glm::vec4& vp)
    : Camera(vp) {
  set_projection(fovy, aspect, near, far);
}

PerspectiveCamera::PerspectiveCamera(const float fovy, const float aspect, const float near, const float far,
                                     glm::vec4&& vp)
    : Camera(std::move(vp)) {
  set_projection(fovy, aspect, near, far);
}

void PerspectiveCamera::set_projection(const float fovy, const float aspect, const float near, const float far) {
  m_projection_matrix = glm::perspective(fovy, aspect, near, far);
}
}  // namespace Yavin