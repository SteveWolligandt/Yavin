#include "OrthographicCamera.h"

#include "gl_includes.h"

#include "windows_undefines.h"

namespace Yavin {
OrthographicCamera::OrthographicCamera(const float left, const float right, const float bottom, const float top,
                                       const float near, const float far, const float vp_x, const float vp_y,
                                       const float vp_width, const float vp_height)
    : Camera(vp_x, vp_y, vp_width, vp_height) {
  set_projection(left, right, bottom, top, near, far);
}

OrthographicCamera::OrthographicCamera(const float left, const float right, const float bottom, const float top,
                                       const float near, const float far, const float vp_width, const float vp_height)
    : Camera(vp_width, vp_height) {
  set_projection(left, right, bottom, top, near, far);
}

OrthographicCamera::OrthographicCamera(const float left, const float right, const float bottom, const float top,
                                       const float near, const float far, const glm::vec4& vp)
    : Camera(vp) {
  set_projection(left, right, bottom, top, near, far);
}

OrthographicCamera::OrthographicCamera(const float left, const float right, const float bottom, const float top,
                                       const float near, const float far, glm::vec4&& vp)
    : Camera(std::move(vp)) {
  set_projection(left, right, bottom, top, near, far);
}

void OrthographicCamera::set_projection(const float left, const float right, const float bottom, const float top,
                                        const float near, const float far) {
  m_projection_matrix = glm::ortho(left, right, bottom, top, near, far);
}
}  // namespace Yavin