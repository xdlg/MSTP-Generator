/**************************************************************************//**
 * @file
 * 
 * Color mapping for SDL.
 *****************************************************************************/

#include "colormap.h"

#define COLOR_DEPTH_8 0xFF

void colormap_RGB888(uint32_t w, uint32_t h, float_t *s, uint32_t *d)
{
    uint8_t pixel;
    uint32_t i;
    
    for (i = 0; i < w*h; i++)
    {
        pixel = (uint8_t)(s[i]*COLOR_DEPTH_8);
        d[i] = (pixel << 16) + (pixel << 8) + pixel;
    }
}

// Maybe lookup tables would be faster?
