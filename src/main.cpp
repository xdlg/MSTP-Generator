/**
 * @file
 * @brief Main
 */

#include "Image.hpp"
#include "PatternGenerator.hpp"
#include "VideoEncoder.hpp"
#include <cstdint>
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv) {
    std::size_t width = 800;
    std::size_t height = 600;
    int bitrate = 400000;
    int framerate = 25;

    VideoEncoder encoder("mpeg4", bitrate, width, height, framerate);

    int ret = encoder.open("output.mpg");
    if (ret < 0) {
        std::cerr << "Cannot open encoder\n";
        return -1;
    }

    PatternGenerator patternGenerator(width, height);

    for (int64_t i = 0; i < 25; i++) {
        ret = encoder.encodeRgbData(patternGenerator.getNextImageData(), i);
        if (ret < 0) {
            std::cerr << "Cannot encode data\n";
            encoder.close();
            std::abort();
        }
    }

    encoder.close();

    return 0;
}
