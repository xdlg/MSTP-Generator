/**
 * @file
 * @brief Image encoded as RGB24 array
 */

#include "Image.hpp"

Image::Image(const size_t width, const size_t height)
    : data(new uint8_t[BYTES_PER_PIXEL * width * height]), width(width), height(height) {}

Image::~Image() {
    delete[] data;
}
