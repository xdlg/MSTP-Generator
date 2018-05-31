/**************************************************************************//**
 * @file
 * 
 * Utility functions.
 *****************************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

/**************************************************************************//**
 * Maximum value in an array.
 * 
 * The number of dimensions of the array doesn't matter, but to avoid warning, 
 * don't forget to (float *)-cast the array parameter when calling the function
 * on a multidimensional array.
 * 
 * @param[in] length Total array length (all dimensions)
 * @param[in] array The array
 * @param[out] max Maximum value
 * 
 * @return Index of the maximum
 *****************************************************************************/
uint32_t max_array(uint32_t length, float *array, float *max);

/**************************************************************************//**
 * Minimum value in an array.
 * 
 * The number of dimensions of the array doesn't matter, but to avoid warning, 
 * don't forget to (float *)-cast the array parameter when calling the function
 * on a multidimensional array.
 * 
 * @param[in] length Total array length (all dimensions)
 * @param[in] array The array
 * @param[out] min Minimum value
 * 
 * @return Index of the minimum
 *****************************************************************************/
uint32_t min_array(uint32_t length, float *array, float *min);

#endif
