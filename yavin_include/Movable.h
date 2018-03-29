#ifndef __YAVIN_MOVABLE_H__
#define __YAVIN_MOVABLE_H__

#include "Transform.h"
#include "dll_export.h"

namespace Yavin {
class Movable {
 public:
  DLL_API Movable()          = default;
  DLL_API virtual ~Movable() = default;
  Transform&       transform() { return m_transform; }
  const Transform& transform() const { return m_transform; }

 protected:
  Transform m_transform;
};
}  // namespace Yavin

#endif