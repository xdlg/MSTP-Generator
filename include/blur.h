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
void blur(size_t w, size_t h, uint32_t r, uint32_t wt, float_t* s, float_t* d);

#endif
