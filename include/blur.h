/**************************************************************************//**
 * @file
 * 
 * Fast box blur.
 *****************************************************************************/

#ifndef BLUR_H
#define BLUR_H

#include <stdint.h>
#include <math.h>

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
void blur(uint32_t w, uint32_t h, uint32_t r, uint32_t wt, float_t *s,
    float_t *d);

#endif
