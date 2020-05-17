/**
 * @file
 * @brief Reads the input CSV file
 */

#include "InputReader.hpp"
#include <iostream>
#include <sstream>

InputReader::InputReader(const std::string& fileName) : file(fileName) {
    if (!file) {
        std::cerr << "Cannot open input file\n";
        std::abort();
    }
}

InputReader::~InputReader() {
    file.close();
}

std::vector<Scale> InputReader::getNextScales() {
    std::vector<Scale> scales;

    if (file.is_open()) {
        std::string line;

        if (std::getline(file, line)) {
            std::stringstream lineStream(line);
            std::string cell;

            while (std::getline(lineStream, cell, ',')) {
                std::size_t activatorRadius = std::stol(cell);

                std::getline(lineStream, cell, ',');
                std::size_t inhibitorRadius = std::stol(cell);

                std::getline(lineStream, cell, ',');
                double increment = std::stod(cell);

                scales.push_back(Scale(activatorRadius, inhibitorRadius, increment));
            }
        }
    }

    return scales;
}
