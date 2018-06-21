/**************************************************************************//**
 * @file
 * 
 * Rotational symmetry functions for the activator/inhibitor arrays.
 *****************************************************************************/

#ifndef SYMMETRY_H
#define SYMMETRY_H

#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <vector>

/**************************************************************************//**
 * Initializes the symmetry lookup tables for a width*height image.
 * 
 * @param[in] w Image width
 * @param[in] h Image height
 *****************************************************************************/
void symmetry_init(const size_t width, const size_t height);

/**************************************************************************//**
 * Applies rotational symmetry to the image.
 * 
 * The symmetry lookup tables must have been initialized beforehand with
 * symmetry_init(). For the moment only symmetry orders 2 and 4 are supported.
 * 
 * @param[in] order Symmetry order
 * @param[in] w Image width
 * @param[in] h Image height
 * @param[inout] im Image
 *****************************************************************************/
void symmetry_apply(const uint32_t order, const size_t w, const size_t h,
    float_t* im);

#endif
