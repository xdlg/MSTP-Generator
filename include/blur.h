/**************************************************************************//**
 * @file
 * 
 * Fast box blur.
 *****************************************************************************/

#ifndef BLUR_H
#define BLUR_H

#include <stdint.h>

/**************************************************************************//**
 * Box blur main function.
 * 
 * @param[in] w Width of the image
 * @param[in] h Height of the image
 * @param[in] r Blurring radius in pixels
 * @param[in] wt Weight
 * @param[in] s Source image
 * @param[out] d Destination image
 *****************************************************************************/
void blur(uint32_t w, uint32_t h, uint32_t r, uint32_t wt, float s[][h],
    float d[][h]);

#endif
