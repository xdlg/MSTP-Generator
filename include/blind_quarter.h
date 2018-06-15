/**************************************************************************//**
 * @file
 * 
 * Multi-scale Turing patterns based on Jonathan McCabe's work.
 *****************************************************************************/

#ifndef BLIND_QUARTER_H
#define BLIND_QUARTER_H

#include <cstdint>
#include <cstdlib>
#include <cmath>
#include "pattern.h"

/**************************************************************************//**
 * Initializes the image with random values.
 * 
 * @param[in] w Image width
 * @param[in] h Image height
 * @param[out] im Image (values in the interval [0; 1])
 *****************************************************************************/
void blind_quarter_init_image(const size_t w, const size_t h, float_t* im);

/**************************************************************************//**
 * One step of the main algorithm.
 * 
 * @param[in] patterns Vector of Turing patterns
 * @param[in] w Image width
 * @param[in] h Image height
 * @param[inout] im Image (values in the interval [0; 1])
 *****************************************************************************/
void blind_quarter_step(pattern_vector& patterns, const size_t w, const size_t h,
    float_t* im);

#endif
