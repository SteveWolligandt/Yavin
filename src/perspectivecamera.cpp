#include <yavin/perspectivecamera.h>

#include <yavin/glincludes.h>

#include <yavin/windowsundefines.h>

//==============================================================================
namespace yavin {
//==============================================================================

perspectivecamera::perspectivecamera(float fovy, float aspect, float near,
                                     float far, size_t vp_x, size_t vp_y,
                                     size_t vp_width, size_t vp_height)
    : camera{vp_x, vp_y, vp_width, vp_height} {
  set_projection(fovy, aspect, near, far);
}

//------------------------------------------------------------------------------

perspectivecamera::perspectivecamera(float fovy, float aspect, float near,
                                     float far, size_t vp_width, size_t vp_height)
    : camera{vp_width, vp_height} {
  set_projection(fovy, aspect, near, far);
}

//------------------------------------------------------------------------------

perspectivecamera::perspectivecamera(float fovy, float aspect, float near,
                                     float far, glm::uvec4& vp)
    : camera{vp} {
  set_projection(fovy, aspect, near, far);
}

//------------------------------------------------------------------------------

perspectivecamera::perspectivecamera(float fovy, float aspect, float near,
                                     float far, glm::uvec4&& vp)
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
                                       float far, size_t vp_width,
                                       size_t vp_height) {
  set_projection(fovy, aspect, near, far);
  set_viewport(vp_width, vp_height);
}

//------------------------------------------------------------------------------

void perspectivecamera::set_projection(float fovy, float aspect, float near,
                                       float far, size_t vp_x, size_t vp_y,
                                       size_t vp_width, size_t vp_height) {
  set_projection(fovy, aspect, near, far);
  set_viewport(vp_x, vp_y, vp_width, vp_height);
}

//==============================================================================
}  // namespace yavin
//==============================================================================
