/**
 * @file
 * @brief Reads the input file
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
