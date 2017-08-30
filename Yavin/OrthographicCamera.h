#ifndef __YAVIN_ORTHOGONAL_CAMERA_H__
#define __YAVIN_ORTHOGONAL_CAMERA_H__

#include "Camera.h"

namespace Yavin {
class OrthographicCamera : public Camera {
 public:
  OrthographicCamera(const float& left, const float& right, const float& bottom, const float& top,
                     const float& near, const float& far);
  void set_projection(const float& left, const float& right, const float& bottom, const float& top,
                      const float& near, const float& far);
};
}  // namespace Yavin

#endif