/**************************************************************************//**
 * @file
 * 
 * Color mapping for SDL.
 *****************************************************************************/

#ifndef COLORMAP_H
#define COLORMAP_H

#include <cstdint>
#include <cstdlib>
#include <cmath>

/** Choice of colormap */
typedef enum
{
    COLORMAP_BW,
    COLORMAP_RAINBOW,
    COLORMAP_LAVA
} colormap_choice;

/**************************************************************************//**
 * Generates the selected colormap.
 * 
 * @param[in] c Choosen colormap
 *****************************************************************************/
void colormap_init(const colormap_choice c);

/**************************************************************************//**
 * Color mapping for SDL pixel format ARGB888.
 * 
 * @param[in] w Image width
 * @param[in] h Image height
 * @param[in] s Source image (pixels in the interval [0; 1])
 * @param[out] d Destination image (format ARGB888)
 *****************************************************************************/
void colormap_ARGB8888(const size_t w, const size_t h, const float_t* s,
    uint32_t* d);

#endif
