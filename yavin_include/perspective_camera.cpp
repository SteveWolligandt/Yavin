#include "perspective_camera.h"

#include "gl_includes.h"

#include "windows_undefines.h"

//==============================================================================
namespace yavin {
//==============================================================================

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near,
                                     float far, float vp_x, float vp_y,
                                     float vp_width, float vp_height)
    : Camera(vp_x, vp_y, vp_width, vp_height) {
  set_projection(fovy, aspect, near, far);
}

//------------------------------------------------------------------------------

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near,
                                     float far, float vp_width, float vp_height)
    : Camera(vp_width, vp_height) {
  set_projection(fovy, aspect, near, far);
}

//------------------------------------------------------------------------------

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near,
                                     float far, glm::vec4& vp)
    : Camera(vp) {
  set_projection(fovy, aspect, near, far);
}

//------------------------------------------------------------------------------

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near,
                                     float far, glm::vec4&& vp)
    : Camera(std::move(vp)) {
  set_projection(fovy, aspect, near, far);
}

//------------------------------------------------------------------------------

void PerspectiveCamera::set_projection(float fovy, float aspect, float near,
                                       float far) {
  m_projection_matrix = glm::perspective(fovy, aspect, near, far);
}

//------------------------------------------------------------------------------

void PerspectiveCamera::set_projection(float fovy, float aspect, float near,
                                       float far, float vp_width,
                                       float vp_height) {
  set_projection(fovy, aspect, near, far);
  set_viewport(vp_width, vp_height);
}

//------------------------------------------------------------------------------

void PerspectiveCamera::set_projection(float fovy, float aspect, float near,
                                       float far, float vp_x, float vp_y,
                                       float vp_width, float vp_height) {
  set_projection(fovy, aspect, near, far);
  set_viewport(vp_x, vp_y, vp_width, vp_height);
}

//==============================================================================
}  // namespace yavin
//==============================================================================
