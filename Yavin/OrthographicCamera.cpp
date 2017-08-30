#include "OrthographicCamera.h"

#include "gl_includes.h"

namespace Yavin {
OrthographicCamera::OrthographicCamera(const float& left, const float& right, const float& bottom,
                                       const float& top, const float& near, const float& far) {
  set_projection(left, right, bottom, top, near, far);
}

void OrthographicCamera::set_projection(const float& left, const float& right, const float& bottom,
                                        const float& top, const float& near, const float& far) {
  m_projection_matrix = glm::ortho(left, right, bottom, top, near, far);
}
}  // Yavin