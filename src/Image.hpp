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
    static constexpr std::size_t BYTES_PER_PIXEL = 3;
    /** @brief Pixel data in RGB24 format (R, G, B, R, G, B, ...) */
    uint8_t* data;

    /**
     * @brief Basic constructor
     * @param[in] width Image width
     * @param[in] height Image height
     */
    Image(std::size_t width, std::size_t height);

    ~Image();

private:
    const std::size_t width;
    const std::size_t height;
};

#endif
