/**
 * @file
 * @brief Pattern generator
 */

#ifndef PATTERN_GENERATOR_HPP
#define PATTERN_GENERATOR_HPP

#include "Scale.hpp"
#include <cstdint>
#include <cstdlib>
#include <vector>

class PatternGenerator {
public:
    /**
     * @brief Basic constructor
     * @param[in] width Generated pattern width
     * @param[in] height Generated pattern height
     */
    PatternGenerator(std::size_t width, std::size_t height);

    ~PatternGenerator();

    /**
     * @brief Updates a pattern
     * @param[in] scales Scales used for generating the pattern
     * @return Pattern data of size width * height on the interval [0;1]
     */
    const double* getNextPattern(std::vector<Scale> scales);

private:
    const std::size_t width;
    const std::size_t height;
    const std::size_t size;
    double* pattern;

    /**
     * @brief Initializes the pattern
     */
    void initializePattern();

    /**
     * @brief Blurs an array
     * @param[in] width Array width
     * @param[in] height Array height
     * @param[in] radius Blurring radius
     * @param[in] source Source array
     * @param[out] destination Destination array
     */
    void blur(std::size_t width, std::size_t height, std::size_t radius, const double* source,
        double* destination) const;

    /**
     * @brief Blurs an array horizontally
     * @param[in] width Array width
     * @param[in] height Array height
     * @param[in] radius Blurring radius
     * @param[in] source Source array
     * @param[out] destination Destination array
     */
    void blurHorizontal(std::size_t width, std::size_t height, std::size_t radius,
        const double* source, double* destination) const;

    /**
     * @brief Blurs an array vertically
     * @param[in] width Array width
     * @param[in] height Array height
     * @param[in] radius Blurring radius
     * @param[in] source Source array
     * @param[out] destination Destination array
     */
    void blurVertical(std::size_t width, std::size_t height, std::size_t radius,
        const double* source, double* destination) const;

    /**
     * @brief Normalizes the pattern to the interval [0; 1]
     */
    void normalizePattern();
};

#endif
