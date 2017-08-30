#ifndef __YAVIN_PERSPECTIVE_CAMERA_H__
#define __YAVIN_PERSPECTIVE_CAMERA_H__

#include "Camera.h"

namespace Yavin {
class PerspectiveCamera : public Camera {
 public:
  PerspectiveCamera(const float& fovy, const float& aspect, const float& near, const float& far);
  void set_projection(const float& fovy, const float& aspect, const float& near, const float& far);
};
}  // namespace Yavin

#endif