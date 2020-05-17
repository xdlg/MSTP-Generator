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
    Scale(std::size_t activatorRadius, std::size_t inhibitorRadius, double increment)
        : activatorRadius(activatorRadius), inhibitorRadius(inhibitorRadius), increment(increment) {
    }

    std::size_t activatorRadius;
    std::size_t inhibitorRadius;
    double increment;
};

#endif
