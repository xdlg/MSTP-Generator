/**
 * @file
 * @brief Reads the input CSV file
 */

#ifndef INPUT_READER_HPP
#define INPUT_READER_HPP

#include <Scale.hpp>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

class InputReader {
public:
    /**
     * @brief Basic constructor
     *
     * Each line of the input file corresponds to one timestamp and must be a list of scale
     * parameters separated by commas.
     * Ex: 100,200,0.05,50,100,0.04 means two scales with respectively 100, 200, 0.05 and 50, 100,
     * 0.04 as activator radius, inhibitor radius, and increment.
     *
     * @param[in] fileName Name of the input file
     */
    InputReader(const std::string& fileName);

    ~InputReader();

    /**
     * @brief Reads the next line from the input file and returns the read scales
     * @return Vector of scales or nullptr if the end of file was reached
     */
    std::vector<Scale> getNextScales();

private:
    std::ifstream file;
};

#endif
