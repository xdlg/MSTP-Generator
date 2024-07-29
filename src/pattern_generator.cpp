#include "pattern_generator.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>
#include <random>
#include <thread>

PatternGenerator::PatternGenerator(std::size_t width, std::size_t height)
  : _width(width), _height(height), _size(width * height), _pattern(std::vector<double>(_size))
{
  randomize();
}

const std::vector<double>& PatternGenerator::pattern() const
{
  return _pattern;
}

void PatternGenerator::randomize()
{
  std::random_device device;
  std::default_random_engine engine(device());
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  std::generate(
    _pattern.begin(), _pattern.end(), [&distribution, &engine]() { return distribution(engine); });
}

void PatternGenerator::normalize()
{
  double min = *std::min_element(_pattern.begin(), _pattern.end());
  double max = *std::max_element(_pattern.begin(), _pattern.end());
  double range = max - min;
  std::transform(_pattern.begin(), _pattern.end(), _pattern.begin(),
    [min, range](double element) { return (element - min) / range; });
}

void PatternGenerator::generate()
{
  if (!_scales.empty()) {
    std::vector<double> activators(_size);
    std::vector<double> inhibitors(_size);
    std::vector<double> variations(_size);
    std::vector<double> increments(_size);

    for (Scale& scale : _scales) {
      // Compute activator and inhibitor arrays
      std::thread inhibitorThread(&PatternGenerator::blur, this, scale._inhibitorRadius,
        std::ref(_pattern), std::ref(inhibitors));
      std::thread activatorThread(&PatternGenerator::blur, this, scale._activatorRadius,
        std::ref(_pattern), std::ref(activators));
      inhibitorThread.join();
      activatorThread.join();

      for (size_t i = 0; i < _size; i++) {
        // Update the variation array if the variation for this element is smaller than the
        // one already stored. When processing the first scale, the variation array is
        // always updated, so we don't need to initialize it beforehand.
        double variation = activators[i] - inhibitors[i];
        if ((&scale == &_scales.front()) || (std::abs(variation) < std::abs(variations[i]))) {
          variations[i] = variation;
          increments[i] = (variation > 0) ? scale._increment : -scale._increment;
        }
      }
    }

    std::transform(
      _pattern.begin(), _pattern.end(), increments.begin(), _pattern.begin(), std::plus<>{});

    normalize();
  }
}

void PatternGenerator::blur(
  std::size_t radius, const std::vector<double>& source, std::vector<double>& destination) const
{
  std::vector<double> partialBlurring(_size);
  blurHorizontal(radius, source, partialBlurring);
  blurVertical(radius, partialBlurring, destination);
}

void PatternGenerator::blurHorizontal(
  std::size_t radius, const std::vector<double>& source, std::vector<double>& destination) const
{
  for (size_t y = 0; y < _height; y++) {
    double sum = 0;
    std::size_t span = radius + 1;

    // In the blurred picture, the first pixel of each row is the average of the source pixels
    // between x = 0 and x = radius
    for (size_t x = 0; x <= radius; x++) {
      sum += source[x + y * _width];
    }
    destination[y * _width] = sum / static_cast<double>(span);

    // The other pixels are computed with a moving average. Pixel values are subtracted or added
    // from the sum only if they are part of the picture.
    for (std::size_t x = 1; x < _width; x++) {
      if ((x + radius) < _width) {
        sum += source[x + radius + y * _width];
      } else {
        span--;
      }

      if ((static_cast<int>(x) - static_cast<int>(radius) - 1) >= 0) {
        sum -= source[x - radius - 1 + y * _width];
      } else {
        span++;
      }

      destination[x + y * _width] = sum / static_cast<double>(span);
    }
  }
}

void PatternGenerator::blurVertical(
  std::size_t radius, const std::vector<double>& source, std::vector<double>& destination) const
{
  for (size_t x = 0; x < _width; x++) {
    double sum = 0;
    std::size_t span = radius + 1;

    // In the blurred picture, the first pixel of each column is the average of the source
    // pixels between y = 0 and y = radius
    for (size_t y = 0; y <= radius; y++) {
      sum += source[x + y * _width];
    }
    destination[x] = sum / static_cast<double>(span);

    // The other pixels are computed with a moving average. Pixel values are subtracted or added
    // from the sum only if they are part of the picture.
    for (std::size_t y = 1; y < _height; y++) {
      if ((y + radius) < _height) {
        sum += source[x + (y + radius) * _width];
      } else {
        span--;
      }

      if ((static_cast<int>(y) - static_cast<int>(radius) - 1) >= 0) {
        sum -= source[x + (y - radius - 1) * _width];
      } else {
        span++;
      }

      destination[x + y * _width] = sum / static_cast<double>(span);
    }
  }
}
