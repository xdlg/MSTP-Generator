/**************************************************************************//**
 * @file
 * Utility functions for Muscatupa project.
 *****************************************************************************/

/******************************************************************************
 * #include
 *****************************************************************************/
#include "utils.h"

/******************************************************************************
 * Private variables
 *****************************************************************************/
 
/******************************************************************************
 * Private functions
 *****************************************************************************/
 
/**************************************************************************//**
 * Maximum value in an array.
 * The number of dimensions of the array doesn't matter, but to avoid warning, 
 * don't forget to (float *)-cast the array parameter when calling the function
 * on a multidimensional array.
 * @param length total array length (all dimensions)
 * @param array the array
 * @param max maximum value (overwritten)
 * @return the index of the maximum
 *****************************************************************************/
uint32_t max_array(uint32_t length, float *array, float *max)
{
	uint32_t i, i_max = 0;
	*max = MAX_INIT;
	
	for (i=0; i<length; i++)
	{
		if (*(array+i) > *max)
		{
			*max = *(array+i);
			i_max = i;
		}
	}
	
	return i_max;
}

/**************************************************************************//**
 * Minimum value in an array.
 * The number of dimensions of the array doesn't matter, but to avoid warning, 
 * don't forget to (float *)-cast the array parameter when calling the function
 * on a multidimensional array.
 * @param length total array length (all dimensions)
 * @param array the array
 * @param min minimum value (overwritten)
 * @return the index of the minimum
 *****************************************************************************/
uint32_t min_array(uint32_t length, float *array, float *min)
{
	uint32_t i, i_min = 0;
	*min = MIN_INIT;
	
	for (i=0; i<length; i++)
	{
		if (*(array+i) < *min)
		{
			*min = *(array+i);
			i_min = i;
		}
	}
	
	return i_min;
}
