/**
 * @file
 * @brief Main
 */

#include "VideoEncoder.hpp"
#include <iostream>

int main(int argc, char** argv) {
    int width = 800;
    int height = 600;
    int bitrate = 400000;
    int framerate = 25;

    VideoEncoder encoder("mpeg4", bitrate, width, height, framerate);

    int ret = encoder.open("output.mpg");
    if (ret < 0) {
        std::cerr << "Cannot open encoder\n";
        return -1;
    }

    uint8_t* rgbData = new uint8_t[3 * width * height];
    for (int64_t j = 0; j < 3 * width * height; j += 3) {
        rgbData[j] = 100;
        rgbData[j + 1] = 0;
        rgbData[j + 2] = 200;
    }

    for (int64_t i = 0; i < 25; i++) {
        ret = encoder.encodeRgbData(rgbData, i);
        if (ret < 0) {
            std::cerr << "Cannot encode data\n";
            encoder.close();
            std::abort();
        }
    }

    delete[] rgbData;

    encoder.close();

    return 0;
}
