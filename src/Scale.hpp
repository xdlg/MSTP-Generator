/**
 * @file
 * @brief Pattern generation scale
 */

#ifndef SCALE_HPP
#define SCALE_HPP

#include <cstdint>
#include <cstdlib>

class Scale {
public:
    Scale(
        bool isEnabled, std::size_t activatorRadius, std::size_t inhibitorRadius, double increment)
        : isEnabled(isEnabled), activatorRadius(activatorRadius), inhibitorRadius(inhibitorRadius),
          increment(increment) {}

    bool isEnabled;
    std::size_t activatorRadius;
    std::size_t inhibitorRadius;
    double increment;
};

#endif
