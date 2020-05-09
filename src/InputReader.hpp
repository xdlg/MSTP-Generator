/**
 * @file
 * @brief Reads the input file
 */

#ifndef INPUT_READER_HPP
#define INPUT_READER_HPP

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <string>

class InputReader {
public:
    /**
     * @brief Basic constructor
     * @param[in] fileName Name of the input file
     */
    InputReader(const std::string& fileName);

    ~InputReader();

private:
    std::ifstream file;
};

#endif
