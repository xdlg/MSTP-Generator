/**
 * @file
 * @brief Image encoded as RGB24 array
 */

#include "Image.hpp"

Image::Image(const size_t width, const size_t height)
    : width(width), height(height), size(BYTES_PER_PIXEL * width * height),
      data(new uint8_t[size]) {}

Image::~Image() {
    delete[] data;
}

const uint8_t* Image::getData() const {
    return data;
}

void Image::colorMapPattern(const double* pattern) {
    for (std::size_t j = 0; j < size; j += BYTES_PER_PIXEL) {
        data[j] = pattern[j / BYTES_PER_PIXEL] * 255.0;
        data[j + 1] = pattern[j / BYTES_PER_PIXEL] * 255.0;
        data[j + 2] = pattern[j / BYTES_PER_PIXEL] * 255.0;
    }
}
