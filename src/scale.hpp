#pragma once

#include <cstdint>
#include <cstdlib>

struct Scale {
  Scale(std::size_t activatorRadius, std::size_t inhibitorRadius, double increment)
    : _activatorRadius(activatorRadius), _inhibitorRadius(inhibitorRadius), _increment(increment)
  {
  }

  std::size_t _activatorRadius;
  std::size_t _inhibitorRadius;
  double _increment;
};
