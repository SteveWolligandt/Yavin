#ifndef __YAVIN_MOVABLE_H__
#define __YAVIN_MOVABLE_H__

#include "Transform.h"

namespace Yavin {
class Movable {
 public:
  Movable()          = default;
  virtual ~Movable() = default;
  Transform&       transform() { return m_transform; }
  const Transform& transform() const { return m_transform; }

 protected:
  Transform m_transform;
};
}  // namespace Yavin

#endif