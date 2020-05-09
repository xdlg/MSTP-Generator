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

int main(int argc, char** argv) {
    std::size_t width = 800;
    std::size_t height = 800;
    int bitrate = 400000;
    int framerate = 25;

    VideoEncoder encoder("mpeg4", bitrate, width, height, framerate);

    int ret = encoder.open("output.mpg");
    if (ret < 0) {
        std::cerr << "Cannot open encoder\n";
        return -1;
    }

    InputReader inputReader("../input.csv");

    PatternGenerator patternGenerator(width, height);
    Scale scale1(true, 100, 200, 0.05);
    patternGenerator.scales.push_back(scale1);
    Scale scale2(true, 50, 100, 0.04);
    patternGenerator.scales.push_back(scale2);
    Scale scale3(true, 25, 50, 0.03);
    patternGenerator.scales.push_back(scale3);
    Scale scale4(true, 12, 25, 0.02);
    patternGenerator.scales.push_back(scale4);
    Scale scale5(true, 6, 12, 0.01);
    patternGenerator.scales.push_back(scale5);

    Image image(width, height);
    std::size_t maxTimestamp = 100;

    for (std::size_t t = 0; t < maxTimestamp; t++) {
        std::cout << "Generating frame " << std::to_string(t + 1) << "/"
                  << std::to_string(maxTimestamp) << "..." << std::endl;

        const double* pattern = patternGenerator.getNextPattern();
        image.colorMapPattern(pattern);

        ret = encoder.encodeRgbData(image.getData(), t);
        if (ret < 0) {
            std::cerr << "Cannot encode data\n";
            encoder.close();
            std::abort();
        }
    }

    encoder.close();

    return 0;
}
