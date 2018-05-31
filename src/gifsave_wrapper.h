/**************************************************************************//**
 * @file
 * 
 * Wrapper for gifsave89.
 *****************************************************************************/

#ifndef GIFSAVE_WRAPPER_H
#define GIFSAVE_WRAPPER_H

#include <stdint.h>

/**************************************************************************//**
 * Initializes the GIF.
 * 
 * @param[in] w Width of the GIF
 * @param[in] h Height of the GIF
 *****************************************************************************/
void gifsave_wrapper_init(uint32_t w, uint32_t h);

/**************************************************************************//**
 * Puts one frame in the GIF buffer.
 * 
 * The frame dimensions must match the width and height parameters given to
 * gifsave_wrapper_init(). To avoid a warning, (float *)-cast the parameter.
 * 
 * @param[in] frame Frame data (2D array) to put in the GIF
 *****************************************************************************/
void gifsave_wrapper_put(uint8_t *frame);

/**************************************************************************//**
 * Closes and saves the GIF.
 *****************************************************************************/
void gifsave_wrapper_close(void);

#endif
