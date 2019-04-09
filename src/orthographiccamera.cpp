#include <yavin/orthographiccamera.h>

#include <yavin/glincludes.h>

#include <yavin/windowsundefines.h>

//==============================================================================
namespace yavin {
//==============================================================================

orthographiccamera::orthographiccamera(const float left, const float right,
                                       const float bottom, const float top,
                                       const float near, const float far,
                                       const float vp_x, const float vp_y,
                                       const float vp_width,
                                       const float vp_height)
    : camera(vp_x, vp_y, vp_width, vp_height) {
  set_projection(left, right, bottom, top, near, far);
}

//------------------------------------------------------------------------------

orthographiccamera::orthographiccamera(const float left, const float right,
                                       const float bottom, const float top,
                                       const float near, const float far,
                                       const float vp_width,
                                       const float vp_height)
    : camera(vp_width, vp_height) {
  set_projection(left, right, bottom, top, near, far);
}

//------------------------------------------------------------------------------

orthographiccamera::orthographiccamera(const float left, const float right,
                                       const float bottom, const float top,
                                       const float near, const float far,
                                       const glm::vec4& vp)
    : camera(vp) {
  set_projection(left, right, bottom, top, near, far);
}

//------------------------------------------------------------------------------

orthographiccamera::orthographiccamera(const float left, const float right,
                                       const float bottom, const float top,
                                       const float near, const float far,
                                       glm::vec4&& vp)
    : camera(std::move(vp)) {
  set_projection(left, right, bottom, top, near, far);
}

//------------------------------------------------------------------------------

void orthographiccamera::set_projection(const float left, const float right,
                                        const float bottom, const float top,
                                        const float near, const float far) {
  m_projection_matrix = glm::ortho(left, right, bottom, top, near, far);
}

//------------------------------------------------------------------------------

void orthographiccamera::set_projection(const float left, const float right,
                                        const float bottom, const float top,
                                        const float near, const float far,
                                        float vp_width, float vp_height) {
  m_projection_matrix = glm::ortho(left, right, bottom, top, near, far);
  set_viewport(vp_width, vp_height);
}

//------------------------------------------------------------------------------

void orthographiccamera::set_projection(const float left, const float right,
                                        const float bottom, const float top,
                                        const float near, const float far,
                                        const float vp_x, const float vp_y,
                                        float vp_width, float vp_height) {
  m_projection_matrix = glm::ortho(left, right, bottom, top, near, far);
  set_viewport(vp_x, vp_y, vp_width, vp_height);
}

//==============================================================================
}  // namespace yavin
//==============================================================================
