/**
 * @file
 * @brief Main
 */

#include "Image.hpp"
#include "InputReader.hpp"
#include "PatternGenerator.hpp"
#include "Scale.hpp"
#include "VideoEncoder.hpp"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    std::size_t width = 854;
    std::size_t height = 480;
    int bitrate = 2500000;
    int framerate = 30;

    VideoEncoder encoder("mpeg4", bitrate, width, height, framerate);

    int ret = encoder.open("output.mpg");
    if (ret < 0) {
        std::cerr << "Cannot open encoder\n";
        return -1;
    }

    PatternGenerator patternGenerator(width, height);
    InputReader inputReader("../input.csv");
    Image image(width, height);
    std::size_t timestamp = 0;

    while (true) {
        std::cout << "Generating frame " << std::to_string(timestamp) << "..." << std::endl;

        std::vector<Scale> scales = inputReader.getNextScales();
        if (scales.empty()) {
            break;
        }

        const double* pattern = patternGenerator.getNextPattern(scales);
        image.colorMapPattern(pattern);

        ret = encoder.encodeRgbData(image.getData(), timestamp);
        if (ret < 0) {
            std::cerr << "Cannot encode data\n";
            encoder.close();
            std::abort();
        }

        timestamp++;
    }

    encoder.close();

    std::cout << "Done\n";

    return 0;
}
