/**
 * @file
 * @brief Pattern generator
 */

#include "PatternGenerator.hpp"
#include <algorithm>
#include <cmath>
#include <random>
#include <thread>

PatternGenerator::PatternGenerator(std::size_t width, std::size_t height)
    : width(width), height(height), size(width * height), pattern(new double[size]) {
    initializePattern();
}

PatternGenerator::~PatternGenerator() {
    delete[] pattern;
}

const double* PatternGenerator::getNextPattern(std::vector<Scale> scales) {
    if (!scales.empty()) {
        double* activators = new double[size];
        double* inhibitors = new double[size];
        double* variations = new double[size];
        double* increments = new double[size];

        for (Scale& scale : scales) {
            // Compute activator and inhibitor arrays
            std::thread inhibitorThread(&PatternGenerator::blur, this, width, height,
                scale.inhibitorRadius, pattern, inhibitors);
            std::thread activatorThread(&PatternGenerator::blur, this, width, height,
                scale.activatorRadius, pattern, activators);
            inhibitorThread.join();
            activatorThread.join();

            for (size_t i = 0; i < size; i++) {
                // Update the variation array if the variation for this element is smaller than the
                // one already stored. When processing the first scale, the variation array is
                // always updated, so we don't need to initialize it beforehand.
                double variation = activators[i] - inhibitors[i];
                if ((&scale == &scales.front())
                    || (std::abs(variation) < std::abs(variations[i]))) {
                    variations[i] = variation;
                    increments[i] = (variation > 0) ? scale.increment : -scale.increment;
                }
            }
        }

        for (size_t i = 0; i < size; i++) {
            pattern[i] += increments[i];
        }

        delete[] activators;
        delete[] inhibitors;
        delete[] variations;
        delete[] increments;

        normalizePattern();
    }

    return pattern;
}

void PatternGenerator::initializePattern() {
    static std::default_random_engine engine;
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (std::size_t i = 0; i < size; i++) {
        pattern[i] = distribution(engine);
    }
}

void PatternGenerator::blur(std::size_t width, std::size_t height, std::size_t radius,
    const double* source, double* destination) const {
    double* partialBlurring = new double[size];

    blurHorizontal(width, height, radius, source, partialBlurring);
    blurVertical(width, height, radius, partialBlurring, destination);

    delete[] partialBlurring;
}

void PatternGenerator::blurHorizontal(std::size_t width, std::size_t height, std::size_t radius,
    const double* source, double* destination) const {
    for (size_t y = 0; y < height; y++) {
        double sum = 0;
        std::size_t span = radius + 1;

        // In the blurred picture, the first pixel of each row is the average of the source pixels
        // between x = 0 and x = radius
        for (size_t x = 0; x <= radius; x++) {
            sum += source[x + y * width];
        }
        destination[y * width] = sum / static_cast<double>(span);

        // The other pixels are computed with a moving average. Pixel values are subtracted or added
        // from the sum only if they are part of the picture.
        for (std::size_t x = 1; x < width; x++) {
            if ((x + radius) < width) {
                sum += source[x + radius + y * width];
            } else {
                span--;
            }

            if ((static_cast<int>(x) - static_cast<int>(radius) - 1) >= 0) {
                sum -= source[x - radius - 1 + y * width];
            } else {
                span++;
            }

            destination[x + y * width] = sum / static_cast<double>(span);
        }
    }
}

void PatternGenerator::blurVertical(std::size_t width, std::size_t height, std::size_t radius,
    const double* source, double* destination) const {
    for (size_t x = 0; x < width; x++) {
        double sum = 0;
        std::size_t span = radius + 1;

        // In the blurred picture, the first pixel of each column is the average of the source
        // pixels between y = 0 and y = radius
        for (size_t y = 0; y <= radius; y++) {
            sum += source[x + y * width];
        }
        destination[x] = sum / static_cast<double>(span);

        // The other pixels are computed with a moving average. Pixel values are subtracted or added
        // from the sum only if they are part of the picture.
        for (std::size_t y = 1; y < height; y++) {
            if ((y + radius) < height) {
                sum += source[x + (y + radius) * width];
            } else {
                span--;
            }

            if ((static_cast<int>(y) - static_cast<int>(radius) - 1) >= 0) {
                sum -= source[x + (y - radius - 1) * width];
            } else {
                span++;
            }

            destination[x + y * width] = sum / static_cast<double>(span);
        }
    }
}

void PatternGenerator::normalizePattern() {
    double min = *std::min_element(pattern, pattern + size);
    double max = *std::max_element(pattern, pattern + size);
    double range = max - min;

    for (size_t i = 0; i < size; i++) {
        pattern[i] = (pattern[i] - min) / range;
    }
}
