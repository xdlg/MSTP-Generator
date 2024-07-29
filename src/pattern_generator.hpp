#pragma once

#include "scale.hpp"

#include <cstdlib>
#include <vector>

class PatternGenerator
{
public:
  /// Scales used to generate the pattern
  std::vector<Scale> _scales;

  /// @param width Image width in pixels
  /// @param height Image height in pixels
  PatternGenerator(std::size_t width, std::size_t height);

  /// Image data of size width*height pixels with values in [0;1]
  const std::vector<double>& pattern() const;

  /// Generate next pattern step
  void generate();

  /// Randomize image data
  void randomize();

private:
  const std::size_t _width;
  const std::size_t _height;
  const std::size_t _size;
  std::vector<double> _pattern;

  /// Normalize image data on the interval [0;1]
  void normalize();

  /// Box blur
  /// @param radius Blur radius
  /// @param source Data to blur
  /// @param destination Blurred data
  void blur(
    std::size_t radius, const std::vector<double>& source, std::vector<double>& destination) const;

  /// Box blur, horizontal pass
  /// @param radius Blur radius
  /// @param source Data to blur
  /// @param destination Blurred data
  void blurHorizontal(
    std::size_t radius, const std::vector<double>& source, std::vector<double>& destination) const;

  /// Box blur, vertical pass
  /// @param radius Blur radius
  /// @param source Data to blur
  /// @param destination Blurred data
  void blurVertical(
    std::size_t radius, const std::vector<double>& source, std::vector<double>& destination) const;
};
