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
 * @param[in] w width of the image
 * @param[in] h height of the image
 * @param[in] r blurring radius in pixels
 * @param[in] wt weight
 * @param[in] s source image
 * @param[out] d destination image
 *****************************************************************************/
void blur(uint32_t w, uint32_t h, uint32_t r, uint32_t wt, float s[][h],
    float d[][h]);

#endif
