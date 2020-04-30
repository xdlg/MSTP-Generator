/**
 * @file
 * @brief Pattern generator
 */

#include "PatternGenerator.hpp"

PatternGenerator::PatternGenerator(std::size_t width, std::size_t height)
    : width(width), height(height), image(width, height), randomGenerator(randomDevice()),
      randomDistribution(0, 0x00FFFFFF) {}

const uint8_t* PatternGenerator::getNextImageData() {
    randomizeImage();
    return image.data;
}

void PatternGenerator::addScale(Scale& scale) {
    scales.push_back(scale);
}

void PatternGenerator::randomizeImage() {
    for (std::size_t j = 0; j < Image::BYTES_PER_PIXEL * width * height;
         j += Image::BYTES_PER_PIXEL) {
        uint32_t randomColor = randomDistribution(randomGenerator);
        image.data[j] = randomColor & 0xFF;
        image.data[j + 1] = (randomColor >> 8) & 0xFF;
        image.data[j + 2] = (randomColor >> 16) & 0xFF;
    }
}
