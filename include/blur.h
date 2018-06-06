/**************************************************************************//**
 * @file
 * 
 * Fast box blur.
 *****************************************************************************/

#ifndef BLUR_H
#define BLUR_H

#include <cstdint>
#include <cstdlib>
#include <cmath>

/**************************************************************************//**
 * Box blur main function.
 * 
 * @param[in] w Image width
 * @param[in] h Image height
 * @param[in] r Blurring radius in pixels
 * @param[in] wt Weight
 * @param[in] s Source image
 * @param[out] d Destination image
 *****************************************************************************/
void blur(const size_t w, const size_t h, const uint32_t r, const uint32_t wt,
    const float_t* s, float_t* d);

#endif
