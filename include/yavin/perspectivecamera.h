#ifndef YAVIN_PERSPECTIVE_CAMERA_H
#define YAVIN_PERSPECTIVE_CAMERA_H

#include "camera.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class perspectivecamera : public camera {
 public:
  DLL_API perspectivecamera(float fovy, float aspect, float near, float far,
                            GLint vp_x, GLint vp_y, GLsizei vp_width,
                            GLsizei vp_height);
  DLL_API perspectivecamera(float fovy, float aspect, float near, float far,
                            GLsizei vp_width, GLsizei vp_height);
  DLL_API void set_projection(float fovy, float aspect, float near, float far);
  DLL_API void set_projection(float fovy, float aspect, float near, float far,
                              GLsizei vp_width, GLsizei vp_height);
  DLL_API void set_projection(float fovy, float aspect, float near, float far,
                              GLint vp_x, GLint vp_y, GLsizei vp_width,
                              GLsizei vp_height);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
