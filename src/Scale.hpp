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
    Scale(bool isEnabled, std::size_t activatorRadius, std::size_t inhibitorRadius, double speed)
        : isEnabled(isEnabled), activatorRadius(activatorRadius), inhibitorRadius(inhibitorRadius),
          speed(speed) {}

    bool isEnabled;
    std::size_t activatorRadius;
    std::size_t inhibitorRadius;
    double speed;
};

#endif
