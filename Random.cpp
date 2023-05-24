#include "Random.h"

namespace util {
Random Random::_instance;

unsigned int Random::getRand(unsigned int max) { return _mt() % max; }

Random::Random() : _dev(), _mt(_dev()) {}
}  // namespace util