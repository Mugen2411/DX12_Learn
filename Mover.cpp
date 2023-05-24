#include "Mover.h"

namespace mover {
bool Mover::Collision(Mover* m) {
  float dx = _x - m->_x;
  float dy = _y - m->_y;
  float r = _radius + m->_radius;
  if (dx * dx + dy * dy <= r * r) return true;
  return false;
}
}  // namespace mover