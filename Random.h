#pragma once
#include <random>

namespace util {
class Random {
 public:
  unsigned int getRand(unsigned int max);
  static Random& getIns() { return _instance;
  }

 private:
  Random();
  std::random_device _dev;
  std::mt19937 _mt;

  static Random _instance;
};
}  // namespace util