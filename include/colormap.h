/**************************************************************************//**
 * @file
 * 
 * Color mapping for SDL.
 *****************************************************************************/

#ifndef COLORMAP_H
#define COLORMAP_H

#include <stdint.h>
#include <math.h>

/**************************************************************************//**
 * Color mapping for SDL pixel format RGB888.
 * 
 * @param[in] w Image width
 * @param[in] h Image height
 * @param[in] s Source image (pixels in the interval [0; 1])
 * @param[out] d Destination image (format RGB888)
 *****************************************************************************/
void colormap_RGB888(uint32_t w, uint32_t h, float_t *s, uint32_t *d);

#endif
