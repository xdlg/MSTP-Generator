/**
 * @file
 * @brief Image encoded as RGB24 array
 */

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstdint>
#include <cstdlib>

class Image {
public:
    /**
     * @brief Basic constructor
     * @param[in] width Image width
     * @param[in] height Image height
     */
    Image(std::size_t width, std::size_t height);

    ~Image();

    /**
     * @brief Gets the image data
     * @return Pixel data in RGB24 format (R, G, B, R, G, B, ...)
     */
    const uint8_t* getData() const;

    /**
     * @brief Color maps a pattern to the image
     * @brief pattern Pattern data in the interval [0; 1]
     */
    void colorMapPattern(const double* pattern);

private:
    static constexpr std::size_t BYTES_PER_PIXEL = 3;

    const std::size_t width;
    const std::size_t height;
    const std::size_t size;
    uint8_t* data;
};

#endif
