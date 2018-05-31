/**************************************************************************//**
 * @file
 * 
 * Colormap generation for gifsave89.
 *****************************************************************************/

#ifndef COLORMAP_H
#define COLORMAP_H

#include <stdint.h>

#define COLOR_DEPTH 256 /**< Number of color levels */

/**************************************************************************//**
 * Builds the RGB colormap for gifsave89.
 * 
 * Modify the called function to select another color scheme, transparently for 
 * the main program.
 * 
 * @param[out] colors Colormap array, must have size 3*COLOR_DEPTH + 1
 *****************************************************************************/
void colormap_build(int32_t *colors);

/**************************************************************************//**
 * Writes the color map to console (debug).
 * 
 * @param[in] colors Colormap array, must have size 3*COLOR_DEPTH + 1
 *****************************************************************************/
void colormap_check(int32_t *map);

#endif
