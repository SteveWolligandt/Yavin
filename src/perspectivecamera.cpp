#include <yavin/glincludes.h>
#include <yavin/perspectivecamera.h>
#include <yavin/windowsundefines.h>
//==============================================================================
namespace yavin {
//==============================================================================
perspectivecamera::perspectivecamera(float fovy, float aspect, float near,
                                     float far, GLint vp_x, GLint vp_y,
                                     GLsizei vp_width, GLsizei vp_height)
    : camera{perspective_matrix(fovy, aspect, near, far), vp_x, vp_y, vp_width,
             vp_height} {}
//------------------------------------------------------------------------------
perspectivecamera::perspectivecamera(float fovy, float aspect, float near,
                                     float far, GLsizei vp_width,
                                     GLsizei vp_height)
    : camera{perspective_matrix(fovy, aspect, near, far), vp_width, vp_height} {
}
//------------------------------------------------------------------------------
void perspectivecamera::set_projection(float fovy, float aspect, float near,
                                       float far) {
  m_projection_matrix = perspective_matrix(fovy, aspect, near, far);
}
//------------------------------------------------------------------------------
void perspectivecamera::set_projection(float fovy, float aspect, float near,
                                       float far, GLsizei vp_width,
                                       GLsizei vp_height) {
  set_projection(fovy, aspect, near, far);
  set_viewport(vp_width, vp_height);
}
//------------------------------------------------------------------------------
void perspectivecamera::set_projection(float fovy, float aspect, float near,
                                       float far, GLint vp_x, GLint vp_y,
                                       GLsizei vp_width, GLsizei vp_height) {
  set_projection(fovy, aspect, near, far);
  set_viewport(vp_x, vp_y, vp_width, vp_height);
}
//==============================================================================
}  // namespace yavin
//==============================================================================
