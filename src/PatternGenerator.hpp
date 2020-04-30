/**
 * @file
 * @brief Pattern generator
 */

#ifndef PATTERN_GENERATOR_HPP
#define PATTERN_GENERATOR_HPP

#include "Image.hpp"
#include <cstdint>
#include <cstdlib>
#include <random>

class PatternGenerator {
public:
    /**
     * @brief Basic constructor
     * @param[in] width Generated image width
     * @param[in] height Generated image height
     */
    PatternGenerator(std::size_t width, std::size_t height);

    /**
     * @brief Gererates the next image and returns the image data
     * @return Pixel data in RGB24 format (R, G, B, R, G, B, ...)
     */
    const uint8_t* getNextImageData();

private:
    std::size_t width;
    std::size_t height;
    Image image;
    std::random_device randomDevice;
    std::mt19937 randomGenerator;
    std::uniform_int_distribution<uint32_t> randomDistribution;

    /**
     * @brief Randomizes the image
     */
    void randomizeImage();
};

#endif
