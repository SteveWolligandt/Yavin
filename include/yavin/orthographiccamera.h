#ifndef __YAVIN_ORTHOGONAL_CAMERA_H__
#define __YAVIN_ORTHOGONAL_CAMERA_H__

#include "camera.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class orthographiccamera : public camera {
 public:
  DLL_API orthographiccamera(const float left, const float right,
                             const float bottom, const float top,
                             const float near, const float far,
                             const float vp_x, const float vp_y,
                             const float vp_width, const float vp_height);

  DLL_API orthographiccamera(const float left, const float right,
                             const float bottom, const float top,
                             const float near, const float far,
                             const float vp_width, const float vp_height);

  DLL_API orthographiccamera(const float left, const float right,
                             const float bottom, const float top,
                             const float near, const float far,
                             const glm::vec4& vp);

  DLL_API orthographiccamera(const float left, const float right,
                             const float bottom, const float top,
                             const float near, const float far, glm::vec4&& vp);

  DLL_API void set_projection(const float left, const float right,
                              const float bottom, const float top,
                              const float near, const float far);
  DLL_API void set_projection(const float left, const float right,
                              const float bottom, const float top,
                              const float near, const float far, float vp_width,
                              float vp_height);
  DLL_API void set_projection(const float left, const float right,
                              const float bottom, const float top,
                              const float near, const float far,
                              const float vp_x, const float vp_y,
                              float vp_width, float vp_height);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif