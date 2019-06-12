#include <yavin/perspectivecamera.h>

#include <yavin/glincludes.h>

#include <yavin/windowsundefines.h>

//==============================================================================
namespace yavin {
//==============================================================================

perspectivecamera::perspectivecamera(float fovy, float aspect, float near,
                                     float far, float vp_x, float vp_y,
                                     float vp_width, float vp_height)
    : camera{vp_x, vp_y, vp_width, vp_height} {
  set_projection(fovy, aspect, near, far);
}

//------------------------------------------------------------------------------

perspectivecamera::perspectivecamera(float fovy, float aspect, float near,
                                     float far, float vp_width, float vp_height)
    : camera{vp_width, vp_height} {
  set_projection(fovy, aspect, near, far);
}

//------------------------------------------------------------------------------

perspectivecamera::perspectivecamera(float fovy, float aspect, float near,
                                     float far, glm::vec4& vp)
    : camera{vp} {
  set_projection(fovy, aspect, near, far);
}

//------------------------------------------------------------------------------

perspectivecamera::perspectivecamera(float fovy, float aspect, float near,
                                     float far, glm::vec4&& vp)
    : camera{std::move(vp)} {
  set_projection(fovy, aspect, near, far);
}

//------------------------------------------------------------------------------

void perspectivecamera::set_projection(float fovy, float aspect, float near,
                                       float far) {
  m_projection_matrix = glm::perspective(fovy, aspect, near, far);
}

//------------------------------------------------------------------------------

void perspectivecamera::set_projection(float fovy, float aspect, float near,
                                       float far, float vp_width,
                                       float vp_height) {
  set_projection(fovy, aspect, near, far);
  set_viewport(vp_width, vp_height);
}

//------------------------------------------------------------------------------

void perspectivecamera::set_projection(float fovy, float aspect, float near,
                                       float far, float vp_x, float vp_y,
                                       float vp_width, float vp_height) {
  set_projection(fovy, aspect, near, far);
  set_viewport(vp_x, vp_y, vp_width, vp_height);
}

//==============================================================================
}  // namespace yavin
//==============================================================================
