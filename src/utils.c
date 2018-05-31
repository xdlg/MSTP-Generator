/**************************************************************************//**
 * @file
 * 
 * Utility functions.
 *****************************************************************************/

#include "utils.h"
#include <float.h>

uint32_t max_array(uint32_t length, float *array, float *max)
{
	uint32_t i;
    uint32_t i_max = 0;
    
	*max = FLT_MIN;
	
	for (i = 0; i < length; i++)
	{
		if (*(array + i) > *max)
		{
			*max = *(array + i);
			i_max = i;
		}
	}
	
	return i_max;
}

uint32_t min_array(uint32_t length, float *array, float *min)
{
	uint32_t i;
    uint32_t i_min = 0;
    
	*min = FLT_MAX;
	
	for (i = 0; i < length; i++)
	{
		if (*(array + i) < *min)
		{
			*min = *(array + i);
			i_min = i;
		}
	}
	
	return i_min;
}
